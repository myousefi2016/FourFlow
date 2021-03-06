/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkSphereSource.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkSphereSource - create a polygonal sphere centered at the origin
// .SECTION Description
// vtkSphereSource creates a sphere (represented by polygons) of specified
// radius centered at the origin. The resolution (polygonal discretization)
// in both the latitude (phi) and longitude (theta) directions can be
// specified. It also is possible to create partial spheres by specifying
// maximum phi and theta angles. By default, the surface tessellation of
// the sphere uses triangles; however you can set LatLongTessellation to
// produce a tessellation using quadrilaterals.
// .SECTION Caveats
// Resolution means the number of latitude or longitude lines for a complete 
// sphere. If you create partial spheres the number of latitude/longitude 
// lines may be off by one. 

#ifndef __vtkRingPolygonSource_h
#define __vtkRingPolygonSource_h

#include "vtkPolyDataAlgorithm.h"
#include "Polygon.h"

class VTK_EXPORT vtkRingPolygonSource : public vtkPolyDataAlgorithm 
{
public:
  vtkTypeMacro(vtkRingPolygonSource,vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  unsigned int  TimeStep;
  vtkSetMacro(TimeStep,unsigned int);
  vtkGetMacro(TimeStep,unsigned int);

  static vtkRingPolygonSource *New();

  vtkSetVector3Macro(Center,double);
  vtkGetVectorMacro(Center,double,3);

  vtkSetMacro(Resolution,int);
  vtkGetMacro(Resolution,int);

  vtkSetStringMacro(Vertex2dString);
  vtkGetStringMacro(Vertex2dString);

  vtkSetStringMacro(InternalName);
  vtkGetStringMacro(InternalName);

protected:
  vtkRingPolygonSource(int res=8);
  ~vtkRingPolygonSource() {}

  double Center[3];
  char* Vertex2dString;
  char* InternalName;

  int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);
  int RequestInformation(vtkInformation *, vtkInformationVector **, vtkInformationVector *);
  //
  // Compute input time steps given the output step
  //
  int RequestUpdateExtent(vtkInformation* request, vtkInformationVector** inputVector,
                                   vtkInformationVector* outputVector);

  Polygon polygon;
  Polygon polygon2d;
  Polygon corners;
  int Resolution;
  
private:
  vtkRingPolygonSource(const vtkRingPolygonSource&);  // Not implemented.
  void operator=(const vtkRingPolygonSource&);  // Not implemented.
  void convertCoordinates(double x, double y, double pos[]);
  bool isInPolygon(float x, float y);
};

#endif
