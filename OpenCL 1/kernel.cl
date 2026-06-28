#include "ocl_image.h"

__kernel void seda( __global OCLImage *t_ocl_img )
{
    size_t x = get_global_id( 0 );
    size_t y = get_global_id( 1 );

    if ( x >= t_ocl_img->m_size.x || y >= t_ocl_img->m_size.y ) return;

    uchar4 pixel = t_ocl_img->at4( y, x );
    
    uchar gray = (uchar)(0.299f * pixel.z + 0.587f * pixel.y + 0.114f * pixel.x);
    
    pixel.x = gray;
    pixel.y = gray;
    pixel.z = gray;
    
    t_ocl_img->at4( y, x ) = pixel;
}

__kernel void negativ( __global OCLImage *t_ocl_img )
{
    size_t x = get_global_id( 0 );
    size_t y = get_global_id( 1 );

    if ( x >= t_ocl_img->m_size.x || y >= t_ocl_img->m_size.y ) return;

    uchar4 pixel = t_ocl_img->at4( y, x );
    
    pixel.x = 255 - pixel.x; // B
    pixel.y = 255 - pixel.y; // G
    pixel.z = 255 - pixel.z; // R
    
    t_ocl_img->at4( y, x ) = pixel;
}

__kernel void kontrast( __global OCLImage *t_ocl_img, float t_contrast )
{
    size_t x = get_global_id( 0 );
    size_t y = get_global_id( 1 );

    if ( x >= t_ocl_img->m_size.x || y >= t_ocl_img->m_size.y ) return;

    uchar4 pixel = t_ocl_img->at4( y, x );
    
    float b = 128.0f + t_contrast * (pixel.x - 128.0f);
    float g = 128.0f + t_contrast * (pixel.y - 128.0f);
    float r = 128.0f + t_contrast * (pixel.z - 128.0f);

    pixel.x = clamp((int)b, 0, 255);
    pixel.y = clamp((int)g, 0, 255);
    pixel.z = clamp((int)r, 0, 255);

    t_ocl_img->at4( y, x ) = pixel;
}

__kernel void zrcadleni( __global OCLImage *t_ocl_in, __global OCLImage *t_ocl_out )
{
    size_t x = get_global_id( 0 );
    size_t y = get_global_id( 1 );

    if ( x >= t_ocl_in->m_size.x || y >= t_ocl_in->m_size.y ) return;

    uchar4 pixel = t_ocl_in->at4( y, x );
    
    size_t new_x = t_ocl_in->m_size.x - 1 - x;
    
    t_ocl_out->at4( y, new_x ) = pixel;
}