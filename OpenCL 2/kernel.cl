#include "ocl_image.h"

inline uchar4 sample_bilinear_rgba( __global OCLImage *img, float x, float y )
{
    if ( x < 0.0f || y < 0.0f || x >= (float)(img->m_size.x - 1) || y >= (float)(img->m_size.y - 1) )
    {
        return (uchar4)( 0, 0, 0, 0 );
    }

    int x0 = (int)x;
    int y0 = (int)y;
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    float dx = x - x0;
    float dy = y - y0;

    uchar4 p00 = img->at4( y0, x0 );
    uchar4 p01 = img->at4( y0, x1 );
    uchar4 p10 = img->at4( y1, x0 );
    uchar4 p11 = img->at4( y1, x1 );

    uchar4 out;
    // B, G, R , A
    out.x = clamp( (int)( p00.x * (1.0f - dx) * (1.0f - dy) + p01.x * dx * (1.0f - dy) + p10.x * (1.0f - dx) * dy + p11.x * dx * dy ), 0, 255 );
    out.y = clamp( (int)( p00.y * (1.0f - dx) * (1.0f - dy) + p01.y * dx * (1.0f - dy) + p10.y * (1.0f - dx) * dy + p11.y * dx * dy ), 0, 255 );
    out.z = clamp( (int)( p00.z * (1.0f - dx) * (1.0f - dy) + p01.z * dx * (1.0f - dy) + p10.z * (1.0f - dx) * dy + p11.z * dx * dy ), 0, 255 );
    out.w = clamp( (int)( p00.w * (1.0f - dx) * (1.0f - dy) + p01.w * dx * (1.0f - dy) + p10.w * (1.0f - dx) * dy + p11.w * dx * dy ), 0, 255 );

    return out;
}

__kernel void generate_background( __global OCLImage *img )
{
    int x = get_global_id( 0 );
    int y = get_global_id( 1 );

    if ( x >= img->m_size.x || y >= img->m_size.y ) return;

    float cx = img->m_size.x * 0.5f;
    float cy = img->m_size.y * 0.5f;

    float nx = (float)x / (float)( img->m_size.x - 1 );
    float ny = (float)y / (float)( img->m_size.y - 1 );

    float dx = (x - cx) / cx;
    float dy = (y - cy) / cy;
    float d = sqrt( dx * dx + dy * dy );

    float vignette = 1.0f - 0.45f * clamp( d, 0.0f, 1.0f );
    int grid = ( (x % 128) < 2 || (y % 128) < 2 ) ? 30 : 0;
    int noise = (x * 20 + y * 29) % 10;

    uchar4 out;
    out.z = clamp( (int)( (60 + 60 * nx + grid + noise) * vignette ), 0, 255 ); // R
    out.y = clamp( (int)( (80 + 80 * ny + grid) * vignette ), 0, 255 );         // G
    out.x = clamp( (int)( (130 + 70 * (1.0f - nx) + grid) * vignette ), 0, 255 ); // B
    out.w = 255; // A

    img->at4( y, x ) = out;
}

__kernel void generate_object( __global OCLImage *obj )
{
    int x = get_global_id( 0 );
    int y = get_global_id( 1 );

    if ( x >= obj->m_size.x || y >= obj->m_size.y ) return;

    float cx = obj->m_size.x * 0.5f;
    float cy = obj->m_size.y * 0.5f;
    float radius = (obj->m_size.x < obj->m_size.y ? obj->m_size.x : obj->m_size.y) * 0.42f;

    float dx = x - cx;
    float dy = y - cy;
    float dist = sqrt( dx * dx + dy * dy );
    float t = dist / radius;

    uchar4 p = { 0, 0, 0, 0 };

    if ( t <= 1.0f )
    {
        float angle = atan2( dy, dx );
        float rays = 0.8f + 0.5f * sin( angle * 7.0f );
        float ring = 0.9f + 0.7f * cos( t * 22.0f );

        p.z = clamp( (int)( 100 + 20 * (1.0f - t) + 40 * rays ), 0, 255 ); // R
        p.y = clamp( (int)( 160 + 50 * ring ), 0, 255 );                   // G
        p.x = clamp( (int)( 240 + 30 * rays ), 0, 255 );                   // B

        float alpha = 1.0f;
        if ( t > 0.75f ) alpha = (1.0f - t) / 0.25f;

        p.w = clamp( (int)( 255.0f * clamp( alpha, 0.0f, 1.0f ) ), 0, 255 ); // A
    }

    obj->at4( y, x ) = p;
}

__kernel void draw_instance( __global OCLImage *background, __global OCLImage *object, float pos_x, float pos_y, float scale, float angle_rad, float mul_r, float mul_g, float mul_b, float global_alpha, int mirror_x, int mirror_y )
{
    int x = get_global_id( 0 );
    int y = get_global_id( 1 );

    if ( x >= background->m_size.x || y >= background->m_size.y ) return;

    float half_w = (object->m_size.x * scale) * 0.5f;
    float half_h = (object->m_size.y * scale) * 0.5f;

    if ( x < pos_x - half_w - 2 || x > pos_x + half_w + 2 || y < pos_y - half_h - 2 || y > pos_y + half_h + 2 ) 
    {
        return;
    }

    float c = cos( angle_rad );
    float s = sin( angle_rad );
    float cx = object->m_size.x * 0.5f;
    float cy = object->m_size.y * 0.5f;

    float dx = x - pos_x;
    float dy = y - pos_y;

    float lx = ( c * dx + s * dy ) / scale;
    float ly = (-s * dx + c * dy ) / scale;

    float sx = lx + cx;
    float sy = ly + cy;

    if ( mirror_x ) sx = (object->m_size.x - 1) - sx;
    if ( mirror_y ) sy = (object->m_size.y - 1) - sy;

    uchar4 src = sample_bilinear_rgba( object, sx, sy );

    if ( src.w == 0 ) return;

    float sb = src.x * mul_b;
    float sg = src.y * mul_g;
    float sr = src.z * mul_r;
    float sa = (src.w / 255.0f) * global_alpha;

    uchar4 bg_pixel = background->at4( y, x );

    bg_pixel.x = clamp( (int)( sb * sa + bg_pixel.x * (1.0f - sa) ), 0, 255 );
    bg_pixel.y = clamp( (int)( sg * sa + bg_pixel.y * (1.0f - sa) ), 0, 255 );
    bg_pixel.z = clamp( (int)( sr * sa + bg_pixel.z * (1.0f - sa) ), 0, 255 );

    background->at4( y, x ) = bg_pixel;
}