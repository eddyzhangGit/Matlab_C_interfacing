//Basic procedure
//This is the basic procedure I follow for setting up a project for Mex interface with matlab, (this is basically my notes, so change as required)
//New C++ project using MacOSX compiler
//Copy over
//mex_interface_tools
//MEXinterfaceTemplate.cpp
//Matlab_shims.h
//Create a main.cpp
//Any other tools / cpp file you know you will need
//Create algorithm class / copy over algorithm code already written
//Map out all modules and the dependency order for coding
//Decide what modules will have test hooks into matlab
//Setup to compile  and test in main
//Setup and test matlab interface with simple command
//Create a matlab build file example :
    p = cd(path_to_files);
    mex - v - D_MATLAB_ - outdir 'path_for_output' file1.cpp file2.cpp
		cd(p)
//Run the tests in matlab
//For each module that will be called
//check any context elements are sync’d
//check no calls to unimplemented matlab functions
//add to alg_c.m
//add to mexinterface.c   param, returns(test the call)
//add the algorithm to alg.c / alg.h
//test in main.c
//test matlab
//Run the full system
//Matlab Interface paramters :
//First parameter is the command(In my design)
//Second parameter is the ctx(In my design)
//All other parameters are param for the function
//Some notes : Matlab matrix are col order and c is row order requires a transpose.
//The Mex interface stuff is pretty tricky,
//1)  nrhs is the number of param
//2)  mxArray *prhs[] is the list of pointers to the parameters
//3)  Most things in matlab are doubles ? (Numeric is for integers[untested])
//4)  mxGetData gets simple data
//5)  For arrays can use load_array in mex_interface_tools.cpp
//Some examples :
//	1)  Get a simple double value
//		double maxShift = *(double*)mxGetData(prhs[4]);
//	2)  Pass one 3x3 rotation matrix
//		dspMatrix rot(3, 3);
//	rot.mxFill(prhs[2]);

//	Matlab interface returns :
//		Context is always the first return (in my design),
//		nlhs is the number of return elements,
//		plhs is the pointers to the mxArray return elements(this is not simply pointers to the data)
//		1)  You must create a mxArray* that points to the mxData.
//		2)  Get the pointer to the data and fill,
//		3)  Everything is a matrix in matlab so a single value is a 1x1 matrix
//		4)  Assign the mxArray* to the mx array list for the return
//		5)  For arrays can use ret_array(this was used for structures so has a name field, can be fixed if needed though)
//		6)  For structures use mxCreateStructMatrix, will need a const char* with the field names.Use mxSetFiledByNumber or mxFieldSetFieldByName to set each individual field with a mxArray*.
//	Example:
//	1)  Return a double value

	mxArray *a = mxCreteDoubleMatrix(1, 1, mxREAL);
	double *p = mxGetPr(a);
	p[0] = double(ret_val);
	plhs[1] = a;
	2)   Use mxCreateNumericArray for integers(untested)
		3)  Structures
		rpm_estimate r = rpm.estimate(A, B);
	mexPrintf("RPM %f   %f\n", r.theta, r.err);
	const char *field_names[] = { "theta", "err" };
	mxArray *a = mxCreateStructMatrix(1, 1, 2, field_names);
	ret_array(a, "theta", 1, 1, 1, 1, &r.theta);
	ret_array(a, "err", 1, 1, 1, 1, &r.err);
	plhs[1] = a

