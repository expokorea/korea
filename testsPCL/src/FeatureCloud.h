/*
 * FeatureCloud.h
 *
 *  Created on: 26/03/2012
 *      Author: arturo
 */

#pragma once

#include <pcl/search/pcl_search.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/features/fpfh.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/features/integral_image_normal.h>
#include "ofConstants.h"

class FeatureCloud
{
  public:
    // A bit of shorthand
    typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;
    typedef pcl::PointCloud<pcl::Normal> SurfaceNormals;
    typedef pcl::PointCloud<pcl::FPFHSignature33> LocalFeatures;
    typedef pcl::search::KdTree<pcl::PointXYZ> SearchMethod;

    FeatureCloud () :
      search_method_xyz_ (new SearchMethod),
      normal_radius_ (0.02),
      feature_radius_ (0.05),
      normalEstimationMethod(pcl::IntegralImageNormalEstimation<pcl::PointXYZ, pcl::Normal>::AVERAGE_3D_GRADIENT),
      maxDepthChangeFactor (0.02),
      normalSmoothingSize(10)
    {}

    ~FeatureCloud () {}

    // Process the given cloud
    void
    setInputCloud (PointCloud::Ptr xyz)
    {
      xyz_ = xyz;
      processInput ();
    }

    // Load and process the cloud in the given PCD file
    void
    loadInputCloud (const std::string &pcd_file)
    {
      xyz_ = PointCloud::Ptr (new PointCloud);
      pcl::io::loadPCDFile (pcd_file, *xyz_);
      processInput ();
    }

    // Get a pointer to the cloud 3D points
    PointCloud::Ptr
    getPointCloud () const
    {
      return (xyz_);
    }

    // Get a pointer to the cloud of 3D surface normals
    SurfaceNormals::Ptr
    getSurfaceNormals () const
    {
      return (normals_);
    }

    // Get a pointer to the cloud of feature descriptors
    LocalFeatures::Ptr
    getLocalFeatures () const
    {
      return (features_);
    }

  protected:
    // Compute the surface normals and local features
    void
    processInput ()
    {
    	computeSurfaceNormals ();
    	computeLocalFeatures ();
    }

    // Compute the surface normals
    void
    computeSurfaceNormals ()
    {
    	cout << "computing normals" << endl;
      normals_ = SurfaceNormals::Ptr (new SurfaceNormals);

      pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> norm_est;
      norm_est.setInputCloud (xyz_);
      norm_est.setSearchMethod (search_method_xyz_);
      norm_est.setRadiusSearch (normal_radius_);
      norm_est.compute (*normals_);
    }

    void
    computeSurfaceNormalsOrdered(){

        normals_ = SurfaceNormals::Ptr (new SurfaceNormals);

    	pcl::IntegralImageNormalEstimation<pcl::PointXYZ, pcl::Normal> norm_est;
    	norm_est.setNormalEstimationMethod (normalEstimationMethod);
    	norm_est.setMaxDepthChangeFactor(maxDepthChangeFactor);
    	norm_est.setNormalSmoothingSize(normalSmoothingSize);
    	norm_est.setInputCloud(xyz_);
    	norm_est.compute(*normals_);
    }

    // Compute the local feature descriptors
    void
    computeLocalFeatures ()
    {
    	cout << "computing features" << endl;
      features_ = LocalFeatures::Ptr (new LocalFeatures);

      pcl::FPFHEstimation<pcl::PointXYZ, pcl::Normal, pcl::FPFHSignature33> fpfh_est;
      fpfh_est.setInputCloud (xyz_);
      fpfh_est.setInputNormals (normals_);
      fpfh_est.setSearchMethod (search_method_xyz_);
      fpfh_est.setRadiusSearch (feature_radius_);
      fpfh_est.compute (*features_);
    }

  private:
    // Point cloud data
    PointCloud::Ptr xyz_;
    SurfaceNormals::Ptr normals_;
    LocalFeatures::Ptr features_;
    SearchMethod::Ptr search_method_xyz_;

    // Parameters
    float normal_radius_;
    float feature_radius_;

    pcl::IntegralImageNormalEstimation<pcl::PointXYZ, pcl::Normal>::NormalEstimationMethod normalEstimationMethod;
    float maxDepthChangeFactor;
    float normalSmoothingSize;
};

