/* To mex, call:
 * mex detect_apriltag_matlabmex.c -I.. ../apriltag.c ../apriltag_quad_thresh.c ../tag36h11.c ../common/g2d.c ../common/image_u8.c ../common/image_u8x3.c ../common/homography.c ../common/workerpool.c ../common/matd.c ../common/pnm.c ../common/svd22.c ../common/zarray.c ../common/zmaxheap.c ../common/time_util.c ../common/string_util.c
 */

#include "mex.h"
#include "apriltag.h"
#include "tag36h11.h"
#include "common/image_u8.h"


void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    /* Declare variables */
    apriltag_family_t* tf;
    apriltag_detector_t* td;
    zarray_t* detections;
    apriltag_detection_t* det;
    double* out;
    int i;
    
    /* Load the image */
    if (nlhs != 1 || nrhs != 1 || mxGetClassID(prhs[0]) != mxUINT8_CLASS || mxIsComplex(prhs[0]) || mxGetNumberOfDimensions(prhs[0]) != 2)
        mexErrMsgTxt("Single input should be a real, 2D uint8 array. Single output expected.");
    image_u8_t im = { .width = mxGetM(prhs[0]), .height = mxGetN(prhs[0]), .stride = mxGetM(prhs[0]), .buf = (uint8_t*)mxGetData(prhs[0]) };
    if (im.width < 240 || im.height < 240)
        mexErrMsgTxt("The input image is too small");
    
    /* Set up the detector */
    tf = tag36h11_create();
    tf->black_border = 0;
    td = apriltag_detector_create();
    apriltag_detector_add_family(td, tf);
    td->quad_decimate = 0;
    td->quad_sigma = 0.5;
    td->nthreads = 1;
    td->debug = 0;
    td->refine_edges = 1;
    td->refine_decode = 1;
    td->refine_pose = 1;

    /* Do the detection */
    detections = apriltag_detector_detect(td, &im);

    /* Output the detections */
    prhs[0] = mxCreateNumericMatrix(12, zarray_size(detections), mxDOUBLE_CLASS, mxREAL); 
    for (i = 0; i < zarray_size(detections); ++i, out += 12) {
        zarray_get_volatile(detections, i, &det);
        out[0] = det->p[0][0];
        out[1] = det->p[0][1];
        out[2] = det->p[1][0];
        out[3] = det->p[1][1];
        out[4] = det->p[2][0];
        out[5] = det->p[2][1];
        out[6] = det->p[3][0];
        out[7] = det->p[3][1];
        out[8] = (double)det->id;
        out[9] = (double)det->hamming;
        out[10] = (double)det->goodness;
        out[11] = (double)det->decision_margin;
    }

    // Deallocations
    apriltag_detections_destroy(detections);
    apriltag_detector_destroy(td);
    tag36h11_destroy(tf);
}
