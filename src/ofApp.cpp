#include "ofApp.h"
#include <vtkImageReader.h>
#include <vtkXMLImageDataReader.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>

#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("Isosurface extraction");
	//readVtiFile();
	gui.setup();
	wireFrame = true;
	computeNormals = false;
	dims = new int[3];
	surfaceOpacity = 0;
	wireFrameOpacity = 0;
	lightingEnabled = false;
	pointLight.setDiffuseColor(ofColor(0.0f,255.0f,0.0f));
	pointLight.setSpecularColor(ofColor(255.0f,255.0f,255.0f));
	pointLight.setPosition(ofGetWidth()*0.5,ofGetHeight()*0.5,0);
	gui.add(fileLoadButton.setup("Load dataset"));
	fileLoadButton.addListener(this, &ofApp::fileSelected);
	fileLoadButton.setBackgroundColor(ofColor::silver);
	//	gui.add(trianglesCount.setup("Triangles count",to_string(triangles.size());
	
	
}

//--------------------------------------------------------------
void ofApp::update() {
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofEnableDepthTest();
	ofPushMatrix();
	ofPushStyle();
	
	camera.begin();
	if(lightingEnabled)
	{
		ofSetSmoothLighting(true);
		ofEnableLighting();
		pointLight.enable();
	}

	if (drawBoundingBox) {
		ofNoFill();
		ofSetColor(ofColor::black);
		ofDrawBox(boundingBoxPosition, boundingBoxSize[0], boundingBoxSize[1], boundingBoxSize[2]);
	}
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1, -1);
	if (wireFrame) {
		ofSetColor(ofColor::lightGreen,wireFrameOpacity);

		mesh.drawWireframe();
	}
	//ofDrawBox(0,0,0,bounds)
	ofSetColor(ofColor::lightYellow,surfaceOpacity);
	mesh.draw();
	camera.end();
	if(lightingEnabled)
	{
		ofDisableLighting();
	}
	ofPopMatrix();
	ofPopStyle();
		
	ofDisableDepthTest();
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::readVtiFile()
{
	vtkSmartPointer<vtkXMLImageDataReader> reader = vtkSmartPointer<vtkXMLImageDataReader>::New();
	reader->SetFileName(filePath.c_str());
	reader->Update();

	image = reader->GetOutput();
	
	image->GetDimensions(dims);

	double *range = new double[2];
	image->GetScalarRange(range);
	minVal = range[0];
	maxVal = range[1];
	data = new float**[dims[0]];
	for (int i = 0;i < dims[0];i++)
	{
		data[i] = new float*[dims[1]];
		for (int j = 0;j < dims[1];j++)
		{
			data[i][j] = new float[dims[2]];
			for (int k = 0; k < dims[2];k++)
			{
				data[i][j][k] = image->GetScalarComponentAsFloat(i, j, k, 0);
			}
		}
	}
}

void ofApp::polygonize()
{
	Cube cube;
	int i, j, k;
	for (i = 0;i<dims[0] - 1;i++) {
		for (j = 0;j<dims[1] - 1;j++) {
			for (k = 0;k<dims[2] - 1;k++) {
				cube.points[0].x = i;
				cube.points[0].y = j;
				cube.points[0].z = k;
				cube.values[0] = data[i][j][k];
				cube.points[1].x = i + 1;
				cube.points[1].y = j;
				cube.points[1].z = k;
				cube.values[1] = data[i + 1][j][k];
				cube.points[2].x = i + 1;
				cube.points[2].y = j + 1;
				cube.points[2].z = k;
				cube.values[2] = data[i + 1][j + 1][k];
				cube.points[3].x = i;
				cube.points[3].y = j + 1;
				cube.points[3].z = k;
				cube.values[3] = data[i][j + 1][k];
				cube.points[4].x = i;
				cube.points[4].y = j;
				cube.points[4].z = k + 1;
				cube.values[4] = data[i][j][k + 1];
				cube.points[5].x = i + 1;
				cube.points[5].y = j;
				cube.points[5].z = k + 1;
				cube.values[5] = data[i + 1][j][k + 1];
				cube.points[6].x = i + 1;
				cube.points[6].y = j + 1;
				cube.points[6].z = k + 1;
				cube.values[6] = data[i + 1][j + 1][k + 1];
				cube.points[7].x = i;
				cube.points[7].y = j + 1;
				cube.points[7].z = k + 1;
				cube.values[7] = data[i][j + 1][k + 1];
				mc->extractIsoSurface(cube, currentIsoValue,i,j,k);
			}
		}
	}
}

void ofApp::isoValueChanged(float &val)
{
	currentIsoValue = val;
	mc->reset();
	polygonize();
	
	createMesh();
}

void ofApp::wireFrameToggled(bool &val)
{
	wireFrame = val;
}

void ofApp::normalsToggled(bool &val)
{
	computeNormals = val;
	if (computeNormals == false) {
		mesh.clearNormals();
		return;
	}
	mc->setComputeNormals(computeNormals);
	mc->reset();
	polygonize();
	createMesh();
}

void ofApp::createMesh()
{
	mesh.clearNormals();
	mesh.clearVertices();
	mesh.clear();
	vector<Triangle> triangles = mc->getTriangles();
	vector<ofVec3f> normals = mc->getNormals();
	int j = 0;
	for (int i = 0; i < triangles.size();i++)
	{
		mesh.addVertex(triangles[i].vertices[0]);
		if (computeNormals)
			mesh.addNormal(normals[j]);
		mesh.addVertex(triangles[i].vertices[1]);
		if (computeNormals)
			mesh.addNormal(normals[j+1]);
		mesh.addVertex(triangles[i].vertices[2]);
		if (computeNormals)
			mesh.addNormal(normals[j+2]);
		mesh.addTriangle(j, j + 1, j + 2);
		j += 3;
	}
}

void ofApp::fileSelected()
{
	ofFileDialogResult result = ofSystemLoadDialog("Load file");
	if (result.bSuccess) {
		filePath = result.getPath();
		ofSetWindowTitle("Isosurface extraction - " + filePath);
	}
	else {
		ofSystemAlertDialog("Invalid file path or could not get file path input correctly!");
		return;
	}
	if(data!=NULL)
		delete data;
	if(mc!=NULL)
		delete mc;
	if(isoSlider!=NULL)
		delete isoSlider;
	if(wireframeToggler!=NULL)
		delete wireframeToggler;
	if(normalsToggler!=NULL)
		delete normalsToggler;
	if (boundingBoxToggler != NULL)
		delete boundingBoxToggler;
	if(wireFrameOpacitySlider!=NULL)
		delete wireFrameOpacitySlider;
	if(surfaceOpacitySlider!= NULL)
		delete surfaceOpacitySlider;
	if(lightingToggler != NULL)
		delete lightingToggler;
	isoSlider = new ofxFloatSlider();
	wireframeToggler = new ofxToggle();
	normalsToggler = new ofxToggle();
	boundingBoxToggler = new ofxToggle();
	wireFrameOpacitySlider = new ofxIntSlider();
	surfaceOpacitySlider = new ofxIntSlider();
	lightingToggler = new ofxToggle();
	lightingToggler->addListener(this,&ofApp::lightingToggled);
	wireFrameOpacitySlider->addListener(this,&ofApp::wireFrameOpacityUpdated);
	surfaceOpacitySlider->addListener(this,&ofApp::surfaceOpacityUpdated);
	wireFrameOpacity = 100;
	surfaceOpacity = 100;
	readVtiFile();
	boundingBoxPosition = ofVec3f(0.0, 0.0, 0.0);
	boundingBoxSize = ofVec3f(dims[0] - 1, dims[1] - 1, dims[2] - 1);
	boundingBoxPosition = 0.5*(boundingBoxSize - boundingBoxPosition);
	camera.setTarget(boundingBoxPosition);
	gui.clear();
	if(gui.getNumControls()==0)
		gui.add(fileLoadButton.setup("Load dataset"));	
	
	gui.add(isoSlider->setup("Isovalue", (minVal + maxVal) / 2.0f, minVal, maxVal));
	isoSlider->addListener(this, &ofApp::isoValueChanged);	
	
	gui.add(wireframeToggler->setup("Wireframe", true));
		
	wireframeToggler->addListener(this, &ofApp::wireFrameToggled);
	
	
	gui.add(normalsToggler->setup("Compute Normals", false));
	normalsToggler->addListener(this, &ofApp::normalsToggled);
	
	gui.add(boundingBoxToggler->setup("Outline box", true));
	boundingBoxToggler->addListener(this, &ofApp::boxToggled);
		gui.add(wireFrameOpacitySlider->setup("Wireframe Opacity",100,0,255));
	gui.add(surfaceOpacitySlider->setup("Surface opacity",100,0,255));
	gui.add(lightingToggler->setup("Lighting",false));

	currentIsoValue = (minVal + maxVal) / 2.0f;
	drawBoundingBox = true;
	computeNormals = false;
	wireFrame = true;
	mc = new MarchingCubes(data, dims);
	//	camera.setAutoDistance(false);
	polygonize();
	vector<Triangle> triangles = mc->getTriangles();
	vector<ofVec3f> normals = mc->getNormals();
	int j = 0;
	mesh.clear();
	for (int i = 0; i < triangles.size();i++)
	{
		mesh.addVertex(triangles[i].vertices[0]);
		if (computeNormals)
			mesh.addNormal(normals[j]);
		mesh.addVertex(triangles[i].vertices[1]);
		if (computeNormals)
			mesh.addNormal(normals[j + 1]);
		mesh.addVertex(triangles[i].vertices[2]);
		if (computeNormals)
			mesh.addNormal(normals[j + 2]);
		mesh.addTriangle(j, j + 1, j + 2);

		
		j += 3;
	}
	//camera.setTarget(mesh);
	ofSetVerticalSync(true);
	
}

void ofApp::boxToggled(bool &val)
{
	if (val == false)
		camera.reset();
	else
		camera.setTarget(boundingBoxPosition);
	drawBoundingBox = val;
}

void ofApp::wireFrameOpacityUpdated(int& val)
{
	wireFrameOpacity = val;
}

void ofApp::surfaceOpacityUpdated(int &val)
{
	surfaceOpacity = val;
}

void ofApp::lightingToggled(bool& val)
{
	lightingEnabled = val;
}
