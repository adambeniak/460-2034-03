
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

cl_int gpu_seda( cl::Program &t_program, OCLImage *t_ocl_img )
{
    cl_int l_err;
    std::string l_kern_name( __FUNCTION__ );
    if ( l_kern_name.find( KERNEL_PREFIX ) == 0 ) l_kern_name.erase( 0, strlen( KERNEL_PREFIX ) );
    
    cl::Kernel l_kern( t_program, l_kern_name.c_str(), &l_err ); CL_ERR_R( l_err );

    l_err = l_kern.setArg( 0, t_ocl_img ); CL_ERR_R( l_err );

    l_kern.setSVMPointers( { t_ocl_img, t_ocl_img->m_data } );

    cl::CommandQueue fronta = cl::CommandQueue::getDefault();
    int l_skupina_x = 16;
    int l_skupina_y = 16;
    int l_celkem_x = ( t_ocl_img->m_size.x + ( l_skupina_x - 1 ) ) / l_skupina_x * l_skupina_x;
    int l_celkem_y = ( t_ocl_img->m_size.y + ( l_skupina_y - 1 ) ) / l_skupina_y * l_skupina_y;

    l_err = fronta.enqueueNDRangeKernel( l_kern, cl::NDRange( 0, 0 ), cl::NDRange( l_celkem_x, l_celkem_y ), cl::NDRange( l_skupina_x, l_skupina_y ) ); CL_ERR_R( l_err );
    fronta.finish();
    return CL_SUCCESS;
}

cl_int gpu_negativ( cl::Program &t_program, OCLImage *t_ocl_img )
{
    cl_int l_err;
    std::string l_kern_name( __FUNCTION__ );
    if ( l_kern_name.find( KERNEL_PREFIX ) == 0 ) l_kern_name.erase( 0, strlen( KERNEL_PREFIX ) );
    
    cl::Kernel l_kern( t_program, l_kern_name.c_str(), &l_err ); CL_ERR_R( l_err );

    l_err = l_kern.setArg( 0, t_ocl_img ); CL_ERR_R( l_err );

    l_kern.setSVMPointers( { t_ocl_img, t_ocl_img->m_data } );

    cl::CommandQueue fronta = cl::CommandQueue::getDefault();
    int l_skupina_x = 16;
    int l_skupina_y = 16;
    int l_celkem_x = ( t_ocl_img->m_size.x + ( l_skupina_x - 1 ) ) / l_skupina_x * l_skupina_x;
    int l_celkem_y = ( t_ocl_img->m_size.y + ( l_skupina_y - 1 ) ) / l_skupina_y * l_skupina_y;

    l_err = fronta.enqueueNDRangeKernel( l_kern, cl::NDRange( 0, 0 ), cl::NDRange( l_celkem_x, l_celkem_y ), cl::NDRange( l_skupina_x, l_skupina_y ) ); CL_ERR_R( l_err );
    fronta.finish();
    return CL_SUCCESS;
}

cl_int gpu_kontrast( cl::Program &t_program, OCLImage *t_ocl_img, float t_contrast )
{
    cl_int l_err;
    std::string l_kern_name( __FUNCTION__ );
    if ( l_kern_name.find( KERNEL_PREFIX ) == 0 ) l_kern_name.erase( 0, strlen( KERNEL_PREFIX ) );
    
    cl::Kernel l_kern( t_program, l_kern_name.c_str(), &l_err ); CL_ERR_R( l_err );

    l_err = l_kern.setArg( 0, t_ocl_img ); CL_ERR_R( l_err );
    l_err = l_kern.setArg( 1, t_contrast ); CL_ERR_R( l_err );

    l_kern.setSVMPointers( { t_ocl_img, t_ocl_img->m_data } );

    cl::CommandQueue fronta = cl::CommandQueue::getDefault();
    int l_skupina_x = 16;
    int l_skupina_y = 16;
    int l_celkem_x = ( t_ocl_img->m_size.x + ( l_skupina_x - 1 ) ) / l_skupina_x * l_skupina_x;
    int l_celkem_y = ( t_ocl_img->m_size.y + ( l_skupina_y - 1 ) ) / l_skupina_y * l_skupina_y;

    l_err = fronta.enqueueNDRangeKernel( l_kern, cl::NDRange( 0, 0 ), cl::NDRange( l_celkem_x, l_celkem_y ), cl::NDRange( l_skupina_x, l_skupina_y ) ); CL_ERR_R( l_err );
    fronta.finish();
    return CL_SUCCESS;
}

cl_int gpu_zrcadleni( cl::Program &t_program, OCLImage *t_ocl_in, OCLImage *t_ocl_out )
{
    cl_int l_err;
    std::string l_kern_name( __FUNCTION__ );
    if ( l_kern_name.find( KERNEL_PREFIX ) == 0 ) l_kern_name.erase( 0, strlen( KERNEL_PREFIX ) );
    
    cl::Kernel l_kern( t_program, l_kern_name.c_str(), &l_err ); CL_ERR_R( l_err );

    l_err = l_kern.setArg( 0, t_ocl_in ); CL_ERR_R( l_err );
    l_err = l_kern.setArg( 1, t_ocl_out ); CL_ERR_R( l_err );

    l_kern.setSVMPointers( { t_ocl_in, t_ocl_in->m_data, t_ocl_out, t_ocl_out->m_data } );

    cl::CommandQueue fronta = cl::CommandQueue::getDefault();
    int l_skupina_x = 16;
    int l_skupina_y = 16;
    int l_celkem_x = ( t_ocl_in->m_size.x + ( l_skupina_x - 1 ) ) / l_skupina_x * l_skupina_x;
    int l_celkem_y = ( t_ocl_in->m_size.y + ( l_skupina_y - 1 ) ) / l_skupina_y * l_skupina_y;

    l_err = fronta.enqueueNDRangeKernel( l_kern, cl::NDRange( 0, 0 ), cl::NDRange( l_celkem_x, l_celkem_y ), cl::NDRange( l_skupina_x, l_skupina_y ) ); CL_ERR_R( l_err );
    fronta.finish();
    return CL_SUCCESS;
}

int main( int t_narg, char **t_args )
{

    cl_int l_err = ocl_init( 1 ); CL_ERR_E( l_err );

    cl::Program l_program( ocl_load_program( KERNEL_SPV ) );

    SVMMatAllocator svmallocator;
    cv::Mat::setDefaultAllocator( &svmallocator );

    cv::Mat l_cv_img = cv::imread( t_args[ 1 ], cv::IMREAD_UNCHANGED );

    if ( l_cv_img.channels() != 4 )
    {
        cv::cvtColor( l_cv_img, l_cv_img, cv::COLOR_BGR2BGRA );
    }

    OCLImage *ocl_puvodni = ocl_svm_malloc< OCLImage >();
    ocl_puvodni->m_size.x = l_cv_img.cols;
    ocl_puvodni->m_size.y = l_cv_img.rows;
    ocl_puvodni->m_data = l_cv_img.data;

    cv::Mat cv_seda = l_cv_img.clone();
    OCLImage *l_ocl_seda = ocl_svm_malloc< OCLImage >();
    l_ocl_seda->m_size.x = cv_seda.cols;
    l_ocl_seda->m_size.y = cv_seda.rows;
    l_ocl_seda->m_data = cv_seda.data;

    cv::Mat cv_neg = l_cv_img.clone();
    OCLImage *l_ocl_neg = ocl_svm_malloc< OCLImage >();
    l_ocl_neg->m_size.x = cv_neg.cols;
    l_ocl_neg->m_size.y = cv_neg.rows;
    l_ocl_neg->m_data = cv_neg.data;

    cv::Mat cv_kontrast = l_cv_img.clone();
    OCLImage *l_ocl_kontrast = ocl_svm_malloc< OCLImage >();
    l_ocl_kontrast->m_size.x = cv_kontrast.cols;
    l_ocl_kontrast->m_size.y = cv_kontrast.rows;
    l_ocl_kontrast->m_data = cv_kontrast.data;

    cv::Mat cv_zrcadleni( l_cv_img.rows, l_cv_img.cols, CV_8UC4 );
    OCLImage *l_ocl_zrcadleni = ocl_svm_malloc< OCLImage >();
    l_ocl_zrcadleni->m_size.x = cv_zrcadleni.cols;
    l_ocl_zrcadleni->m_size.y = cv_zrcadleni.rows;
    l_ocl_zrcadleni->m_data = cv_zrcadleni.data;

    gpu_seda( l_program, l_ocl_seda );
    gpu_negativ( l_program, l_ocl_neg );
    gpu_kontrast( l_program, l_ocl_kontrast, 1.5f );
    gpu_zrcadleni( l_program, ocl_puvodni, l_ocl_zrcadleni );

    cv::imshow( "0. Original", l_cv_img );
    cv::imshow( "1. Seda", cv_seda );
    cv::imshow( "2. Negativ", cv_neg );
    cv::imshow( "3. Kontrast", cv_kontrast );
    cv::imshow( "4. Zrcadleni", cv_zrcadleni );

    cv::waitKey( 0 );

    return 0;
}