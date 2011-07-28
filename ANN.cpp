// ANN.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include "ml.h"
#include <fstream>
#include <string>
#include <iostream>
using namespace std;
using namespace cv;

float* init_array(int size){
	float* arr;
	arr= new float[size];
	for (int i=0;i<size;i++){
		arr[i]=0;
	}
	return arr;
}

int max_index(float x []){
	int index;
	float value=-100;
	for (int i=1;i<x[0]+1;i++){
		
		if (x[i]>value){
			index=i;
			value=x[i];
		}
	}
	return index;
}

void build_mlp_classifier(){
	int number_input_node=180;
	int number_output_node=12;
	int numTraindata=171;
	int index=0;
	int hidden_layers=36;
	int test_case=66;
	int size=30;

	CvMat* input_mat;
	input_mat = cvCreateMat( numTraindata, number_input_node, CV_32FC1 );
	
	string line;
	ifstream myfile ("train.txt");
	if (myfile.is_open()){
		while ( myfile.good() ){
			IplImage* src;
			getline (myfile,line);  
			const char * filename = line.c_str();
			cout<<filename<<endl;
			src=cvLoadImage(filename,1);
			float* r_h= init_array(size);
			float* g_h= init_array(size);
			float* b_h= init_array(size);
			float* r_v= init_array(size);
			float* g_v= init_array(size);
			float* b_v= init_array(size);
			float total_value=0;
		
			for (int x=0;x<src->height;x++){
				for (int y=0;y<src->width;y++){ 
					CvScalar p = cvGet2D(src,x,y);
					double b = p.val[0];
					double g = p.val[1];
					double r = p.val[2];
					r_h[x]+=r;
					g_h[x]+=g;
					b_h[x]+=b;
					r_v[y]+=r;
					g_v[y]+=g;
					b_v[y]+=b;
					total_value=total_value+r+g+b;
				}	
			}


			for (int x=0;x<src->height;x++){
				r_h[x]=r_h[x]/total_value;
				g_h[x]=g_h[x]/total_value;
				b_h[x]=b_h[x]/total_value;
				r_v[x]=r_v[x]/total_value;
				g_v[x]=g_v[x]/total_value;
				b_v[x]=b_v[x]/total_value;

			}
			
			for (int k=0;k<size;k++){
				*( (float*)CV_MAT_ELEM_PTR( *input_mat, index,k) ) =r_h[k];

			}

			for (int f=0;f<size;f++){
				*( (float*)CV_MAT_ELEM_PTR( *input_mat, index,f+30,  ) ) =g_h[f];
				
			}

			for (int g=0;g<size;g++){
				*( (float*)CV_MAT_ELEM_PTR( *input_mat, index,g+60 ) ) =b_h[g];
			}


			for (int k=0;k<size;k++){
				*( (float*)CV_MAT_ELEM_PTR( *input_mat, index,k+90) ) =r_v[k];
				
			}

			for (int f=0;f<size;f++){
				*( (float*)CV_MAT_ELEM_PTR( *input_mat, index,f+120,  ) ) =g_v[f];
			}

			for (int g=0;g<size;g++){
				*( (float*)CV_MAT_ELEM_PTR( *input_mat, index,g+150 ) ) =b_v[g];
			}
			index++;
			cvReleaseImage(&src );
		}
		
		myfile.close();
	}else 
		cout << "Unable to open file"; 

	CvMat* output_mat;
	output_mat = cvCreateMat( numTraindata, number_output_node, CV_32FC1 );
	string line1;
	int num_id=0;
	ifstream myfile1 ("train_result.txt");
	if (myfile1.is_open()){
	    while ( myfile1.good() ){
			getline (myfile1,line1);  
			const char * filename = line1.c_str();
			int id=atoi(const_cast<char *> (filename));
			
			
			for (int i=0;i<number_output_node;i++){
				int value;
				if (i==id-1){value=2;}
				else {value=0;}
				*( (float*)CV_MAT_ELEM_PTR( *output_mat,num_id,i) ) =value;
			}
			num_id++;
		}
		myfile1.close();
	}
	
	CvANN_MLP mpl;
	CvANN_MLP_TrainParams params;
	CvTermCriteria criteria;
	criteria.max_iter = 1000;
	criteria.epsilon = 0.00001f;
	criteria.type = CV_TERMCRIT_ITER | CV_TERMCRIT_EPS;

	params.train_method = CvANN_MLP_TrainParams::BACKPROP;
	params.bp_dw_scale = 0.05f;
	params.bp_moment_scale = 0.05f;
	params.term_crit = criteria;
	
	int layer_num[3] = { number_input_node,hidden_layers, number_output_node }; 
	CvMat *layer_size = cvCreateMatHeader( 1, 3, CV_32S );
	cvInitMatHeader( layer_size, 1, 3, CV_32S, layer_num ); 
	

	mpl.create(layer_size,CvANN_MLP::SIGMOID_SYM, 0,0 );	
	mpl.train( input_mat, output_mat, 0, 0, params );
	mpl.save( "NN_DATA.xml" );


	CvMat* tst_mat;
	tst_mat = cvCreateMat( test_case, number_input_node, CV_32FC1 );
	CvMat* tst_res ;
	tst_res = cvCreateMat( test_case, number_output_node, CV_32FC1 );


	index=0;
	string line2;
	ifstream myfile2 ("test.txt");
	if (myfile2.is_open()){
		while ( myfile2.good() ){
			IplImage* src;
			getline (myfile2,line2);  
			const char * filename = line2.c_str();
			src=cvLoadImage(filename,1);
			float* r_h= init_array(size);
			float* g_h= init_array(size);
			float* b_h= init_array(size);
			float* r_v= init_array(size);
			float* g_v= init_array(size);
			float* b_v= init_array(size);
			float total_value=0;
			for (int x=0;x<src->height;x++){
				for (int y=0;y<src->width;y++){
					CvScalar p = cvGet2D(src,x,y);
					double b = p.val[0];
					double g = p.val[1];
					double r = p.val[2];
					r_h[x]+=r;
					g_h[x]+=g;
					b_h[x]+=b;
					r_v[y]+=r;
					g_v[y]+=g;
					b_v[y]+=b;
					total_value=total_value+r+g+b;
				}	
			}



			for (int x=0;x<src->height;x++){
				r_h[x]=r_h[x]/total_value;
				g_h[x]=g_h[x]/total_value;
				b_h[x]=b_h[x]/total_value;
				r_v[x]=r_v[x]/total_value;
				g_v[x]=g_v[x]/total_value;
				b_v[x]=b_v[x]/total_value;

			}
			
			for (int k=0;k<30;k++){
				*( (float*)CV_MAT_ELEM_PTR( *tst_mat, index,k) ) =r_h[k];

			}

			for (int f=0;f<30;f++){
				*( (float*)CV_MAT_ELEM_PTR( *tst_mat, index,f+size,  ) ) = g_h[f];
			}

			for (int g=0;g<30;g++){
				*( (float*)CV_MAT_ELEM_PTR( *tst_mat, index,g+size*2 ) ) = b_h[g];
			}


			for (int k=0;k<30;k++){
				*( (float*)CV_MAT_ELEM_PTR( *tst_mat, index,k+size*3) ) = r_v[k];
				
			}

			for (int f=0;f<30;f++){
				*( (float*)CV_MAT_ELEM_PTR( *tst_mat, index,f+size*4  ) ) = g_v[f];
			}

			for (int g=0;g<30;g++){
				*( (float*)CV_MAT_ELEM_PTR( *tst_mat, index,g+size*5) ) = b_v[g];
			}
			index++;
			cvReleaseImage(&src );
		}
		
		myfile.close();
	}else 
		cout << "Unable to open file"; 


	mpl.predict(tst_mat,tst_res);
	string line4;
    ifstream myfile12 ("test_result.txt");
  
	int success=0;
	int s=0;
	for (int p=0;p<test_case;p++){ 
		float* res;
		res = new float [number_output_node+1];
		res[0]=number_output_node;
		for (int h=0;h<number_output_node;h++){
			res[h+1]=CV_MAT_ELEM( *tst_res, float, p, h );
		}
		
		getline (myfile12,line4);  
		const char * filename = line4.c_str();
		int i=atoi(const_cast<char *> (filename));
		int r=max_index(res);
		cout<<"Predicted: "<<r<<" Actual: "<<i<<endl;
		if (i==r)success++;
		s++;
	}
	float s_r=(float)success/(float)test_case;
	cout<<success<<endl;
	cout<<s<<endl;
	cout<<"success rate: "<<s_r;
	myfile12.close();
}



int main(int argc, char** argv)
{

	build_mlp_classifier();
	cvWaitKey();
	int i;
	cin>>i;
	return 0;
}

