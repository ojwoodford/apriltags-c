function compile_mex()
mex detect_apriltag_matlabmex.c -I.. ../apriltag.c ../apriltag_quad_thresh.c ../tag36h11.c ../common/workerpool.c ../common/matd.c ../common/image_u8.c ../common/image_u8x3.c ../common/homography.c ../common/svd22.c ../common/zmaxheap.c ../common/zarray.c ../common/string_util.c ../common/pnm.c ../common/time_util.c ../common/g2d.c
end