
#include <cstdlib>
#include <iostream>
#include <math.h>

#include <opencv2/opencv.hpp>
#include <CL/opencl.hpp>

#include "ocl_utils.h"
#include "ocl_image.h"
#include "ocl_svm_mat_allocator.h"

#define KERNEL_SPV      "kernel.spv"
#define KERNEL_PREFIX   "gpu_"

const int SIRKA_POZADI = 1024;
const int VYSKA_POZADI = 768;
const int SIRKA_OBJEKTU = 128;
const int VYSKA_OBJEKTU = 128;

cl_int gpu_generate_background( cl::Program &program, OCLImage *pozadi_img )
{
    cl_int l_err;
    std::string kern_name( __FUNCTION__ );
    if ( kern_name.find( KERNEL_PREFIX ) == 0 ) kern_name.erase( 0, strlen( KERNEL_PREFIX ) );
    
    cl::Kernel kernel( program, kern_name.c_str(), &l_err ); CL_ERR_R( l_err );
    kernel.setArg( 0, pozadi_img ); 
    kernel.setSVMPointers( { pozadi_img, pozadi_img->m_data } );

    cl::CommandQueue fronta = cl::CommandQueue::getDefault();
    int skupina_x = 16, skupina_y = 16;
    int mrizka_x = ( pozadi_img->m_size.x + ( skupina_x - 1 ) ) / skupina_x * skupina_x;
    int mrizka_y = ( pozadi_img->m_size.y + ( skupina_y - 1 ) ) / skupina_y * skupina_y;

    l_err = fronta.enqueueNDRangeKernel( kernel, cl::NDRange( 0, 0 ), cl::NDRange( mrizka_x, mrizka_y ), cl::NDRange( skupina_x, skupina_y ) );
    fronta.finish();
    return CL_SUCCESS;
}

cl_int gpu_generate_object( cl::Program &program, OCLImage *objekt_img )
{
    cl_int l_err;
    std::string kern_name( __FUNCTION__ );
    if ( kern_name.find( KERNEL_PREFIX ) == 0 ) kern_name.erase( 0, strlen( KERNEL_PREFIX ) );
    
    cl::Kernel kernel( program, kern_name.c_str(), &l_err ); CL_ERR_R( l_err );
    kernel.setArg( 0, objekt_img ); 
    kernel.setSVMPointers( { objekt_img, objekt_img->m_data } );

    cl::CommandQueue fronta = cl::CommandQueue::getDefault();
    int skupina_x = 16, skupina_y = 16;
    int mrizka_x = ( objekt_img->m_size.x + ( skupina_x - 1 ) ) / skupina_x * skupina_x;
    int mrizka_y = ( objekt_img->m_size.y + ( skupina_y - 1 ) ) / skupina_y * skupina_y;

    l_err = fronta.enqueueNDRangeKernel( kernel, cl::NDRange( 0, 0 ), cl::NDRange( mrizka_x, mrizka_y ), cl::NDRange( skupina_x, skupina_y ) );
    fronta.finish();
    return CL_SUCCESS;
}

cl_int gpu_draw_instance( cl::Program &program, OCLImage *pozadi_img, OCLImage *objekt_img, float p_x, float p_y, float meritko, float uhel, float mul_r, float mul_g, float mul_b, float alfa, int zrc_x, int zrc_y )
{
    cl_int l_err;
    std::string kern_name( __FUNCTION__ );
    if ( kern_name.find( KERNEL_PREFIX ) == 0 ) kern_name.erase( 0, strlen( KERNEL_PREFIX ) );
    
    cl::Kernel kernel( program, kern_name.c_str(), &l_err ); CL_ERR_R( l_err );
    
    kernel.setArg( 0, pozadi_img );
    kernel.setArg( 1, objekt_img );
    kernel.setArg( 2, p_x );
    kernel.setArg( 3, p_y );
    kernel.setArg( 4, meritko );
    kernel.setArg( 5, uhel );
    kernel.setArg( 6, mul_r );
    kernel.setArg( 7, mul_g );
    kernel.setArg( 8, mul_b );
    kernel.setArg( 9, alfa );
    kernel.setArg( 10, zrc_x );
    kernel.setArg( 11, zrc_y );

    kernel.setSVMPointers( { pozadi_img, pozadi_img->m_data, objekt_img, objekt_img->m_data } );

    cl::CommandQueue fronta = cl::CommandQueue::getDefault();
    int skupina_x = 16, skupina_y = 16;
    
    int mrizka_x = ( pozadi_img->m_size.x + ( skupina_x - 1 ) ) / skupina_x * skupina_x;
    int mrizka_y = ( pozadi_img->m_size.y + ( skupina_y - 1 ) ) / skupina_y * skupina_y;

    l_err = fronta.enqueueNDRangeKernel( kernel, cl::NDRange( 0, 0 ), cl::NDRange( mrizka_x, mrizka_y ), cl::NDRange( skupina_x, skupina_y ) );
    fronta.finish(); 
    return CL_SUCCESS;
}

int main( int argc, char **argv )
{
    cl_int l_err = ocl_init( 1 ); CL_ERR_E( l_err );

    cl::Program program( ocl_load_program( KERNEL_SPV ) );

    SVMMatAllocator svm_alokator;
    cv::Mat::setDefaultAllocator( &svm_alokator );

    cv::Mat cv_pozadi( VYSKA_POZADI, SIRKA_POZADI, CV_8UC4 );
    OCLImage *ocl_pozadi = ocl_svm_malloc< OCLImage >();
    ocl_pozadi->m_size.x = cv_pozadi.cols;
    ocl_pozadi->m_size.y = cv_pozadi.rows;
    ocl_pozadi->m_data = cv_pozadi.data;

    cv::Mat cv_objekt( VYSKA_OBJEKTU, SIRKA_OBJEKTU, CV_8UC4 );
    OCLImage *ocl_objekt = ocl_svm_malloc< OCLImage >();
    ocl_objekt->m_size.x = cv_objekt.cols;
    ocl_objekt->m_size.y = cv_objekt.rows;
    ocl_objekt->m_data = cv_objekt.data;

    gpu_generate_background( program, ocl_pozadi );
    gpu_generate_object( program, ocl_objekt );

    for ( int i = 0; i < 20; i++ ) {
        float x = 150.0f + (float)( (i * 112) % (SIRKA_POZADI - 105) );
        float y = 150.0f + (float)( (i * 45)  % (VYSKA_POZADI - 105) );

        float meritko = 0.6f + 0.04f * (i % 4);
        float uhel = 0.81f * i;

        float mul_r = 0.9f + 0.2f * (i % 3);
        float mul_g = 0.1f + 0.8f * ((i + 4) % 2);
        float mul_b = 0.5f + 0.4f * ((i + 4) % 6);

        float alfa = 0.55f + 0.02f * (i % 10);

        int zrcadli_x = (i % 3 == 0);
        int zrcadli_y = (i % 5 == 0);

        gpu_draw_instance( program, ocl_pozadi, ocl_objekt, x, y, meritko, uhel, mul_r, mul_g, mul_b, alfa, zrcadli_x, zrcadli_y );
    }

    cv::imshow( "Vysledek", cv_pozadi );
    
    cv::waitKey( 0 );

    return 0;
}