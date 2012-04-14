#include "testApp.h"
#undef Success
#include "ofxPCL.h"
#include "FeatureCloud.h"

#include "TemplateAlignment.h"
#include <pcl/keypoints/harris_keypoint3D.h>
#include <pcl/search/pcl_search.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/features/fpfh.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/registration/icp.h>

pcl::PointCloud<pcl::PointXYZ>::Ptr pc(new pcl::PointCloud<pcl::PointXYZ>);
pcl::PointCloud<pcl::PointXYZ>::Ptr pcFiltered(new pcl::PointCloud<pcl::PointXYZ>);
pcl::PointCloud<pcl::PointXYZ>::Ptr pcAligned(new pcl::PointCloud<pcl::PointXYZ>);
pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> n;
pcl::PointCloud<pcl::Normal>::Ptr normals;
FeatureCloud featureCloud1;
FeatureCloud featureCloud2;
TemplateAlignment templateAlignment;
pcl::IterativeClosestPoint<pcl::PointXYZ, pcl::PointXYZ> icp;
TemplateAlignment::Result result;

//pcl::HarrisKeypoint3D< pcl::PointXYZRGB, pcl::PointXYZRGB, pcl::Normal > keypointDetector;

typedef pcl::search::KdTree<pcl::PointXYZ> SearchMethod;

pcl::PointCloud<pcl::Normal>::Ptr calculateNormals(pcl::PointCloud<pcl::PointXYZ>::Ptr & cloud, float normal_radius_=0.02){
    SearchMethod::Ptr search_method_xyz_;
    pcl::PointCloud<pcl::Normal>::Ptr normals_;
	normals_ = pcl::PointCloud<pcl::Normal>::Ptr (new pcl::PointCloud<pcl::Normal>);

	pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> norm_est;
	norm_est.setInputCloud (cloud);
	norm_est.setSearchMethod (search_method_xyz_);
	norm_est.setRadiusSearch (normal_radius_);
	norm_est.compute (*normals_);

	return normals_;
}


pcl::PointCloud<pcl::Normal>::Ptr calculateNormalsOrdered(pcl::PointCloud<pcl::PointXYZ>::Ptr & cloud, pcl::IntegralImageNormalEstimation<pcl::PointXYZ, pcl::Normal>::NormalEstimationMethod normalEstimationMethod=pcl::IntegralImageNormalEstimation<pcl::PointXYZ, pcl::Normal>::AVERAGE_3D_GRADIENT,float maxDepthChangeFactor=0.02, float normalSmoothingSize=10){
	pcl::PointCloud<pcl::Normal>::Ptr normals (new pcl::PointCloud<pcl::Normal> ());

	pcl::IntegralImageNormalEstimation<pcl::PointXYZ, pcl::Normal> ne;
	ne.setNormalEstimationMethod (normalEstimationMethod);
	ne.setMaxDepthChangeFactor(maxDepthChangeFactor);
	ne.setNormalSmoothingSize(normalSmoothingSize);
	ne.setInputCloud(cloud);
	ne.compute(*normals);

	return normals;
}

typedef pcl::PointCloud<pcl::FPFHSignature33> LocalFeatures;
LocalFeatures::Ptr
computeLocalFeatures (const pcl::PointCloud<pcl::PointXYZ>::Ptr & cloudIn, const pcl::PointCloud<pcl::Normal>::Ptr & normals,float feature_radius=.05)
{
  LocalFeatures::Ptr features;
  SearchMethod::Ptr search_method_xyz_;
  features = LocalFeatures::Ptr (new LocalFeatures);


  pcl::FPFHEstimation<pcl::PointXYZ, pcl::Normal, pcl::FPFHSignature33> fpfh_est;
  fpfh_est.setInputCloud (cloudIn);
  fpfh_est.setInputNormals (normals);
  fpfh_est.setSearchMethod (search_method_xyz_);
  fpfh_est.setRadiusSearch (feature_radius);
  fpfh_est.compute (*features);
  return features;
}

void voxelGridFilter(const pcl::PointCloud<pcl::PointXYZ>::Ptr & cloudIn, pcl::PointCloud<pcl::PointXYZ> & cloudOut ){
	pcl::VoxelGrid<pcl::PointXYZ> p;
	p.setInputCloud (cloudIn);
	//p.setFilterLimits (0.0, 0.5);
	//p.setFilterFieldName ("z");
	p.setLeafSize (0.005, 0.005, 0.005);
	p.filter(cloudOut);
}

void statisticalOutlierFilter(const pcl::PointCloud<pcl::PointXYZ>::Ptr & cloudIn, pcl::PointCloud<pcl::PointXYZ> & cloudOut ){
	pcl::StatisticalOutlierRemoval<pcl::PointXYZ> p;
	p.setInputCloud (cloudIn);
	p.setMeanK (50);
	p.setStddevMulThresh (1.0);
	p.filter(cloudOut);
}

void removeDistanteFilter(const pcl::PointCloud<pcl::PointXYZ>::Ptr & cloudIn, pcl::PointCloud<pcl::PointXYZ> & cloudOut , float depth_limit = 1.0){
	  pcl::PassThrough<pcl::PointXYZ> pass;
	  pass.setInputCloud (cloudIn);
	  pass.setFilterFieldName ("z");
	  pass.setFilterLimits (0, depth_limit);
	  pass.filter (cloudOut);
}

//--------------------------------------------------------------
void testApp::setup(){
	kinect.init();
	kinect.open();
	kinect.setRegistration(true);
	/*pc->resize(640*480);
	pc->width = 640;
	pc->height = 480;*/
	pc->is_dense = true;
	/*pcFiltered->resize(640*480);
	pcFiltered->width = 640;
	pcFiltered->height = 480;*/
	pcFiltered->is_dense = true;
	/*mesh.getVertices().resize(640*480);
	mesh.getColors().resize(640*480);*/
	mesh1.setMode(OF_PRIMITIVE_POINTS);
	mesh2.setMode(OF_PRIMITIVE_POINTS);
	meshorig.setMode(OF_PRIMITIVE_POINTS);
	//mesh.setUsage(GL_STREAM_DRAW);
	normalsMesh.setMode(OF_PRIMITIVE_LINES);
	normalsMesh.setUsage(GL_STREAM_DRAW);
	gui.setup();
	gui.add(voxelFilter.set("voxelFilter",false));
	gui.add(removeDistantePoints.set("removeDistantePoints",false));
	gui.add(computeNormals.set("computeNormals",false));
	gui.add(showNormals.set("showNormals",false));
	gui.add(computeFeatures.set("computeFeatures",false));
	gui.add(draw1.set("draw1",false));
	gui.add(draw2.set("draw2",false));
	gui.add(draworig.set("draworig",false));
	gui.add(align.set("align",false));
	gui.add(maxDistance.set("maxDistance",20,0,20));
	gui.add(rot.set("rot",0,-180,180));
	gui.add(templates.set("templates",0,0,5));

	//featureCloud1.loadInputCloud("cloud2.pcd");
	//featureCloud2.loadInputCloud("cloud1.pcd");

	pcl::io::loadPCDFile ("cloud2.pcd", *pc);
	pcl::io::loadPCDFile ("cloud1.pcd", *pcFiltered);

	for(int i=0;i<pcFiltered->size();i++){
		const ofVec3f v((*pcFiltered)[i].x*1000,(*pcFiltered)[i].y*1000,(*pcFiltered)[i].z*1000);
		meshorig.addVertex(v);
		//mesh.addColor( ofFloatColor(v.z/6000.,v.z/6000.,v.z/6000.) );
		//mesh.addColor(kinect.getPixelsRef().getColor((*pcFiltered)[i].x*1000,(*pcFiltered)[i].y*1000));
	}
	icp.setInputCloud (pc);
	icp.setInputTarget (pcFiltered);
	icp.setMaximumIterations (500);
	icp.setMaxCorrespondenceDistance (0.01);
    // Set the maximum number of iterations (criterion 1)
    icp.setMaximumIterations (200);
    // Set the transformation epsilon (criterion 2)
    icp.setTransformationEpsilon (1e-8);
    // Set the euclidean distance difference epsilon (criterion 3)
    icp.setEuclideanFitnessEpsilon (1);
	icp.align (*pcAligned);
	pcl::transformPointCloud (*pcFiltered, *pcFiltered, icp.getFinalTransformation());
	//icp.getFinalTransformation();


	//templateAlignment.setTargetCloud(featureCloud1);
	//templateAlignment.align(featureCloud2,result);


	cout << "transforming" << endl;
	//pcl::transformPointCloud (*featureCloud2.getPointCloud (), *pc, result.final_transformation);

	cout << " to mesh " << endl;

	/*pcl::io::loadPCDFile ("cloud2.pcd", *pc);
	mesh.clear();*/
	/*mesh.clear();
	for(int i=0;i<featureCloud1.getPointCloud()->size();i++){
		const ofVec3f v((*featureCloud1.getPointCloud())[i].x*1000,(*featureCloud1.getPointCloud())[i].y*1000,(*featureCloud1.getPointCloud())[i].z*1000);
		mesh.addVertex(v);
		//mesh.addColor( ofFloatColor(v.z/6000.,v.z/6000.,v.z/6000.) );
		//mesh.addColor(kinect.getPixelsRef().getColor((*pcFiltered)[i].x*1000,(*pcFiltered)[i].y*1000));
	}*/
	mesh2.clear();
	for(int i=0;i<pc->size();i++){
		const ofVec3f v((*pc)[i].x*1000,(*pc)[i].y*1000,(*pc)[i].z*1000);
		mesh2.addVertex(v);
		//mesh.addColor( ofFloatColor(v.z/6000.,v.z/6000.,v.z/6000.) );
		//mesh.addColor(kinect.getPixelsRef().getColor((*pcFiltered)[i].x*1000,(*pcFiltered)[i].y*1000));
	}
	/*for(int i=0;i<pcFiltered->size();i++){
		const ofVec3f v((*pcFiltered)[i].x*1000,(*pcFiltered)[i].y*1000,(*pcFiltered)[i].z*1000);
		mesh2.addVertex(v);
		//mesh.addColor( ofFloatColor(v.z/6000.,v.z/6000.,v.z/6000.) );
		//mesh.addColor(kinect.getPixelsRef().getColor((*pcFiltered)[i].x*1000,(*pcFiltered)[i].y*1000));
	}*/
	//templates.addListener(this,&testApp::loadTemplate);

	/*pcl::io::loadPCDFile (ofToDataPath("person.pcd"), *pc);
	removeDistanteFilter(pc,*pcFiltered,1);
	voxelGridFilter(pcFiltered,*pcFiltered);
	//pc = featureCloud.getPointCloud();

	mesh.clear();
	for(int i=0;i<pcFiltered->size();i++){
		const ofVec3f v((*pcFiltered)[i].x*1000,(*pcFiltered)[i].y*1000,(*pcFiltered)[i].z*1000);
		mesh.addVertex(v);
		//mesh.addColor( ofFloatColor(v.z/6000.,v.z/6000.,v.z/6000.) );
		//mesh.addColor(kinect.getPixelsRef().getColor((*pcFiltered)[i].x*1000,(*pcFiltered)[i].y*1000));
	}*/
}

void testApp::loadTemplate(int & templateN){

	//featureCloud.loadInputCloud(ofToDataPath("object_template_"+ofToString(templateN)+".pcd"));
	//pc = featureCloud.getPointCloud();

	mesh1.clear();
	for(int i=0;i<pc->size();i++){
		const ofVec3f v((*pc)[i].x*1000,(*pc)[i].y*1000,(*pc)[i].z*1000);
		mesh1.addVertex(v);
		//mesh.addColor( ofFloatColor(v.z/6000.,v.z/6000.,v.z/6000.) );
		//mesh.addColor(kinect.getPixelsRef().getColor((*pcFiltered)[i].x*1000,(*pcFiltered)[i].y*1000));
	}

}

//--------------------------------------------------------------
void testApp::update(){
	static int frame=0;
	kinect.update();

	if(kinect.isFrameNew()){
		frame++;
		int i = 0;
		pc->clear();
		pcFiltered->clear();
		for(int y=0;y<480;y++){
			for(int x=0;x<640;x++){
				const ofVec3f & v= kinect.getWorldCoordinateAt(x,y);
				if(v!=ofVec3f(0,0,0)){
					(*pc).push_back(pcl::PointXYZ(v.x*.001,v.y*.001,v.z*.001));
				}
				i++;
			}
		}
		removeDistanteFilter(pc,*pcFiltered,maxDistance);
		voxelGridFilter(pcFiltered,*pcFiltered);

		if(align)pcl::transformPointCloud (*pcFiltered, *pcFiltered, icp.getFinalTransformation());
		/*if(frame==20){
			pcl::io::savePCDFile("cloud1.pcd",*pcFiltered,true);
			std::exit(0);
		}*/


		//featureCloud.setInputCloud(pcFiltered);

		/*if(removeDistantePoints){
			removeDistanteFilter(pc,1);
		}
		if(voxelFilter){
			voxelGridFilter(pc,*(pcFiltered.get()));
		}
		bool filtered = voxelFilter || removeDistantePoints;

		if(computeNormals){
			if(!filtered) normals = calculateNormalsOrdered(pc);
			else normals = calculateNormals(pcFiltered);
			if(computeFeatures){
				LocalFeatures::Ptr features = computeLocalFeatures(pcFiltered,normals);
			}
		}*/


		if(true){
			mesh1.clear();
			for(int i=0;i<pcFiltered->size();i++){
				ofVec3f v((*pcFiltered)[i].x*1000,(*pcFiltered)[i].y*1000,(*pcFiltered)[i].z*1000);
				mesh1.addVertex(v);
				//mesh.addColor( ofFloatColor(v.z/6000.,v.z/6000.,v.z/6000.) );
				//mesh1.addColor(kinect.getPixelsRef().getColor((*pcFiltered)[i].x*1000,(*pcFiltered)[i].y*1000));
			}
		}else{
			mesh1.getVertices().resize(640*480);
			mesh1.getColors().resize(640*480);
			i = 0;
			for(int y=0;y<480;y++){
				for(int x=0;x<640;x++){
					const ofVec3f & v = kinect.getWorldCoordinateAt(x,y);
					mesh1.getVertices( )[i] = v;
					//mesh.getColors()[i] = ofFloatColor(v.z/6000.,v.z/6000.,v.z/6000.);
					mesh1.getColors()[i] = kinect.getPixelsRef().getColor(x,y);
					i++;
				}
			}
		}

		/*
*/

		if(showNormals){
			normalsMesh.clear();
			for(int i=0;i<normals->size();i+=10){
				normalsMesh.addVertex(mesh1.getVertices()[i]);
				normalsMesh.addVertex(mesh1.getVertices()[i]+ofVec3f(normals->at(i).normal_x,normals->at(i).normal_y,normals->at(i).normal_z)*100);

				normalsMesh.addColor(kinect.getPixelsRef().getColor(mesh1.getVertices()[i].x,mesh1.getVertices()[i].y));
				normalsMesh.addColor(kinect.getPixelsRef().getColor(mesh1.getVertices()[i].x,mesh1.getVertices()[i].y));
				//cout << ofVec3f(normals->at(i).normal_x,normals->at(i).normal_y,normals->at(i).normal_z) << endl;
			}
		}
		//cout << "end normals" << endl << endl << endl;
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	gui.draw();
	//cam.begin();
	/*ofTranslate(0,0,maxDistance*1000);
	ofTranslate(0,0,300);
	ofRotate(rot,0,1,0);
	ofTranslate(0,0,-300);*/
	ofScale(1,1,-1);

	if(showNormals){
		normalsMesh.draw();
	}else{
		if(draw1)mesh1.draw();
		if(draw2)mesh2.draw();
		if(draworig)meshorig.draw();
	}
	//normalsMesh.draw();
	//cam.end();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
