#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <stdlib.h> //For sleep, i didnt like how sensitive commands are

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <csci441/shader.h>
#include <csci441/matrix.h>
#include <csci441/vector.h>
#include <csci441/uniform.h>

#include "shape.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Derek Wallace

//Used for stamp and primitive transforms
struct stampData {
	double red;
	double blue;
	double green;
	double rotate;
	double translateX;
	double translateY;
	double scaleX;
	double scaleY;
	double size;
};

//Cut down user input into usable info for transformations
//Probably easier way to do thihs
stampData getStampData(std::string data) {
	std::string delimiter = ":";
	std::string point = data.substr(0, data.find(delimiter)); //point should be r,g,b
	data.erase(0, data.find(delimiter) + delimiter.length());
	std::string rotate = data.substr(0, data.find(delimiter)); //rotate
	data.erase(0, data.find(delimiter) + delimiter.length());
	std::string translate = data.substr(0, data.find(delimiter)); //transX,transY
	data.erase(0, data.find(delimiter) + delimiter.length());
	std::string scale = data.substr(0, data.find(delimiter)); //scalex,scaley
	data.erase(0, data.find(delimiter) + delimiter.length());
	std::string size = data.substr(0, data.find(delimiter)); //size
	data.erase(0, data.find(delimiter) + delimiter.length());
	delimiter = ",";
	std::string r = point.substr(0, point.find(delimiter));	//r is red
	point.erase(0, point.find(delimiter) + delimiter.length());
	std::string g = point.substr(0, point.find(delimiter));
	point.erase(0, point.find(delimiter) + delimiter.length());
	std::string b = point;

	std::string tx = translate.substr(0, translate.find(delimiter));
	translate.erase(0, translate.find(delimiter) + delimiter.length());
	std::string ty = translate;

	std::string sx = scale.substr(0, scale.find(delimiter));
	scale.erase(0, scale.find(delimiter) + delimiter.length());
	std::string sy = scale;

	stampData myShape;
	myShape.red = atof(r.c_str());
	myShape.green = atof(g.c_str());
	myShape.blue = atof(b.c_str());
	myShape.translateX= atof(tx.c_str());
	myShape.translateY= atof(ty.c_str());
	myShape.scaleX=atof(sx.c_str());
	myShape.scaleY= atof(sy.c_str());
	myShape.size= atof(size.c_str());;
	myShape.rotate = atof(rotate.c_str());
	return myShape;
}

//How i apply the transformations to single shapes
std::vector<float> transform(float* values, std::vector<float> coords) {
	std::vector<float> newCoords;
	for (int i = 0; i < coords.size(); i=i+6) {
		//needs to change if normals are added, 6 is x,y,z,r,g,b
		for (int j = 0; j < 12; j = j + 4) {
			//j=0 is x val, j=4 is y, j=8 is z
			float x = values[j] * coords[i] + values[j + 1] * coords[i + 1] + values[j + 2] * coords[i + 2] + values[12 + j/4];
			newCoords.push_back(x);
		}
		//add rgb values
		newCoords.push_back(coords[i + 3]);
		newCoords.push_back(coords[i + 4]);
		newCoords.push_back(coords[i + 5]);
	}

	return newCoords;
}

//used to change color in Modify mode
std::vector<float> changeColor(float* values, std::vector<float> coords) {
	std::vector<float> newCoords;
	for (int i = 0; i < coords.size(); i = i + 6) {
		newCoords.push_back(coords[i]);
		newCoords.push_back(coords[i+1]);
		newCoords.push_back(coords[i+2]);
		newCoords.push_back(values[0]);
		newCoords.push_back(values[1]);
		newCoords.push_back(values[2]);
	}

	return newCoords;
}
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

bool isPressed(GLFWwindow *window, int key) {
    return glfwGetKey(window, key) == GLFW_PRESS;
}

bool isReleased(GLFWwindow *window, int key) {
    return glfwGetKey(window, key) == GLFW_RELEASE;
}

Matrix processModel(const Matrix& model, GLFWwindow *window) {
    Matrix trans;

    const float ROT = 1;
    const float SCALE = .005;
    const float TRANS = .02;

    // ROTATE (Not recommended, messes with mouse controls
    if (isPressed(window, '[')) { trans.rotate_z(-ROT); }
    else if (isPressed(window, ']')) { trans.rotate_z(ROT); }
    // SCALE
    else if (isPressed(window, '-')) { trans.scale(1-SCALE, 1-SCALE, 1-SCALE); }
    else if (isPressed(window, '=')) { trans.scale(1+SCALE, 1+SCALE, 1+SCALE); }
    // TRANSLATE
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
		double xpos, ypos;
		int x, y;
		glfwGetWindowSize(window, &x, &y);
		glfwGetCursorPos(window, &xpos, &ypos);
		//make math easier
		xpos = xpos - (x / 2);
		ypos = ypos - (y / 2);
		//Introduce a scaling factor to make farther inputs mean slide farther
		double xS = 2*abs(xpos / (x/2));
		double yS = 2*abs(ypos / (y / 2));
		//Which way to move
		if ((xpos > 0 ) && (ypos < 0)) { trans.translate(xS*TRANS, yS*TRANS, 0); }
		else if ((xpos < 0) && (ypos < 0)) { trans.translate(-xS*TRANS, yS*TRANS, 0); }
		else if ((xpos > 0) && (ypos > 0)) { trans.translate(xS*TRANS, -yS*TRANS, 0); }
		else { trans.translate(-xS*TRANS, -yS*TRANS, 0); }
	}
	Sleep(5); //Just enough delay but still fast
    return trans * model;
}



void processInput(Matrix& model, GLFWwindow *window) {
    if (isPressed(window, GLFW_KEY_ESCAPE) || isPressed(window, GLFW_KEY_Q)) {
        glfwSetWindowShouldClose(window, true);
    }
    model = processModel(model, window);
}

void errorCallback(int error, const char* description) {
    fprintf(stderr, "GLFW Error: %s\n", description);
}

int main(void) {
	GLFWwindow* window;

	glfwSetErrorCallback(errorCallback);

	/* Initialize the library */
	if (!glfwInit()) { return -1; }

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Program1", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// tell glfw what to do on resize
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	// init glad
	if (!gladLoadGL()) {
		std::cerr << "Failed to initialize OpenGL context" << std::endl;
		glfwTerminate();
		return -1;
	}

	//testing shapes, not used currently
	Circle c(30,.25,0,0,1,.5,-.5);
	Triangle tri(.25 ,	1, 0, 0,0,0);

	//for use with subbuffers, to track where things are
	int buffSize = 0;
	
	// copy vertex data
	GLuint VBO[1];
	glGenBuffers(1, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	
	glBufferData(GL_ARRAY_BUFFER,10000, 0, GL_STATIC_DRAW); //Big buffer bc i don't know how much will be added by user

	// describe vertex layout
	//leftover from testing, trying to phase out safely
	GLuint circleVAO;	
	GLuint triVAO;
	glGenVertexArrays(1, &circleVAO);
	glGenVertexArrays(1, &triVAO);
	glBindVertexArray(circleVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),(void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(triVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
		(void*)(c.coords.size()*sizeof(float)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
		(void*)(c.coords.size() * sizeof(float) + 3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	

	// setup projection
	Matrix projection; 
	projection.perspective(45, 1, .01, 10);
	
	int r,t;
	glfwGetWindowSize(window, &r, &t);	//get window size, used for resizing
	float asp = r / t;
	

	Vector eye(0, 0, 2);
	Vector origin(0, 0, 0);
	Vector up(0, 1, 0);

	Matrix camera;
	camera.look_at(eye, origin, up);

	// create the shaders
	//Shader shader("../vert.glsl", "../frag.glsl");
	Shader shader("C:/Users/nolla/Desktop/Graphics Projects/csci441-spring2018/programs/program1/vert.glsl", "C:/Users/nolla/Desktop/Graphics Projects/csci441-spring2018/programs/program1/frag.glsl"); //my path

	// setup the textures
	shader.use();

	// set the matrices
	Matrix model;

	int state = 0; //0=view, 1=stamp, 2=UDP, 3=Group, 4=Modify
	//used to track last shape for modifying
	GLuint lastShape; 
	float vertexSize;
	std::vector<float> lastCoords;

    // and use z-buffering
    glEnable(GL_DEPTH_TEST);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        // process input
		if (isPressed(window, GLFW_KEY_V)) {
			state = 0;
			//Must be in view to exit with ESC
		}
		else if (isPressed(window, GLFW_KEY_P)) {
			state = 1;
		}
		else if (isPressed(window, GLFW_KEY_U)) {
			state = 2;
		}
		else if (isPressed(window, GLFW_KEY_G)) {
			state = 3;
		}
		else if (isPressed(window, GLFW_KEY_M)) {
			state = 4;
		}
		if (state == 0) {
			processInput(model, window);
		}
        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // activate shader
        shader.use();

		//maintain aspect ratio, kind of works
		glfwGetWindowSize(window, &r, &t);	//get window size, used for keeping aspect ratio uniform
		float asp = r / t;
		projection.perspective(20, asp, .01, 10);//works for now, fix later?
		glViewport(0,0,r,t);	//needed?
		
		//stamp keys (s for square, c for circle, t for triangle)
		if (state == 1) {
			if (isPressed(window, GLFW_KEY_S)) {
				double x, y;
				glfwGetCursorPos(window, &x, &y);
				glfwGetWindowSize(window, &r, &t);
				x = -1.0 + x * (2.0 / r);
				y = 1.0 - y * (2.0 / t);
				Sleep(500);
				std::string data;
				std::cout << "Enter attributes (enter as r,g,b: rotate: translate x, translate y: scale x, scale y: size): " << std::endl;
				std::cin >> data;

				stampData shape = getStampData(data);
				//My square is actually same algorithm as circle, just with n=4
				Square s(shape.size, shape.red, shape.green, shape.blue, x, y);
				
				//transforms new shape
				Matrix trans, rot, scale, finTrans; //for ease of use, track entered transformations
				rot.rotate_z(shape.rotate);
				trans.translate(shape.translateX, shape.translateY, 0);
				scale.scale(shape.scaleX, shape.scaleY, 0);
				finTrans = rot * trans*scale * finTrans;
				s.coords = transform(finTrans.values, s.coords); //WORKING!!!

				//setup sub buffer and draw shape
				GLuint sVAO;
				glBufferSubData(GL_ARRAY_BUFFER, buffSize, s.coords.size() * sizeof(float), &s.coords[0]);
				buffSize = buffSize + s.coords.size() * sizeof(float);
				glGenVertexArrays(1, &sVAO);
				glBindVertexArray(sVAO);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
				glEnableVertexAttribArray(1);

				glDrawArrays(GL_TRIANGLES, 0, s.coords.size() * sizeof(float));

				//used by modify to track last made shape
				lastCoords = s.coords;
				vertexSize = s.coords.size() * sizeof(float);
				lastShape = sVAO;
			}

			//TODO: break repeated code down into methods 
			else if (isPressed(window, GLFW_KEY_C)) {
				double x, y;
				glfwGetCursorPos(window, &x, &y);
				glfwGetWindowSize(window, &r, &t);
				x = -1.0 + x * (2.0 / r);
				y = 1.0 - y * (2.0 / t);
				Sleep(500);
				std::string data;
				std::cout << "Circles are same when rotated, rotate option changed for n gon generator, n>20 makes circles " << std::endl;
				std::cout << "Enter attributes (enter as r,g,b: Number of sides: translate x, translate y: scale x, scale y: size): " << std::endl;
				std::cin >> data;

				stampData shape = getStampData(data);
				//my circle is actually an n gon generator, ie n=5 makes a pentagon
				Circle s(shape.rotate, shape.size, shape.red, shape.green, shape.blue, x, y);

				//Transform
				Matrix trans, rot, scale, finTrans; 
				rot.rotate_z(shape.rotate);
				trans.translate(shape.translateX, shape.translateY, 0);
				scale.scale(shape.scaleX, shape.scaleY, 0);
				finTrans = rot * trans*scale * finTrans;
				s.coords = transform(finTrans.values, s.coords); 

				GLuint sVAO;
				glBufferSubData(GL_ARRAY_BUFFER, buffSize, s.coords.size() * sizeof(float), &s.coords[0]);
				buffSize = buffSize + s.coords.size() * sizeof(float);
				glGenVertexArrays(1, &sVAO);
				glBindVertexArray(sVAO);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
				glEnableVertexAttribArray(1);

				glDrawArrays(GL_TRIANGLES, 0, s.coords.size() * sizeof(float));

				lastCoords = s.coords;
				vertexSize = s.coords.size() * sizeof(float);
				lastShape = sVAO;
			}
			else if (isPressed(window, GLFW_KEY_T)) {
				double x, y;
				glfwGetCursorPos(window, &x, &y);
				glfwGetWindowSize(window, &r, &t);
				x = -1.0 + x * (2.0 / r);
				y = 1.0 - y * (2.0 / t);
				Sleep(500);
				std::string data;
				std::cout << "Enter attributes (enter as as r,g,b: rotate: translate x, translate y: scale x, scale y: size): " << std::endl;
				std::cin >> data;

				stampData shape = getStampData(data);
				Triangle s(shape.size, shape.red, shape.green, shape.blue, x, y);

				Matrix trans, rot, scale, finTrans; 
				rot.rotate_z(shape.rotate);
				trans.translate(shape.translateX, shape.translateY, 0);
				scale.scale(shape.scaleX, shape.scaleY, 0);
				finTrans = rot * trans*scale * finTrans;
				s.coords = transform(finTrans.values, s.coords); 

				GLuint sVAO;
				glBufferSubData(GL_ARRAY_BUFFER, buffSize, s.coords.size() * sizeof(float), &s.coords[0]);
				buffSize = buffSize + s.coords.size() * sizeof(float); //tracks size of buffer offset
				glGenVertexArrays(1, &sVAO);
				glBindVertexArray(sVAO);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
				glEnableVertexAttribArray(1);

				glDrawArrays(GL_TRIANGLES, buffSize, s.coords.size() * sizeof(float));
				lastCoords = s.coords;
				vertexSize = s.coords.size() * sizeof(float);
				lastShape = sVAO;
			}
		}

		//User defined Primitive
		else if (state == 2) {
			if (isPressed(window, GLFW_KEY_N)) {
				std::string data;
				std::cout << "Enter attributes (enter as r,g,b: rotate: translate x, translate y: scale x, scale y: size): " << std::endl;
				std::cin >> data;
				stampData shape = getStampData(data);
				std::vector<float> coords;
				bool stopper = false;
				std::string delimiter = ",";
				while (stopper==false) {
					if (data == "q") {
						stopper = true;
					}
					else{
						//Couldnt get mouse input working for point selection without freezing window, coordinates must be typed in
						std::cout << "Enter point (enter as x,y): " << std::endl;
						std::cin >> data;
						
						std::string point = data.substr(0, data.find(delimiter)); //point should be x,y
						data.erase(0, data.find(delimiter) + delimiter.length());
						double x= atof(point.c_str());
						double y= atof(data.c_str());
						coords.push_back(x);
						coords.push_back(y);
						coords.push_back(0);
						coords.push_back(shape.red);
						coords.push_back(shape.green);
						coords.push_back(shape.blue);
						std::cout << "Points in shape: " << coords.size()/6 << std::endl; //helpful feedback so you know how many points youve made
					}
				
				}
				
				Matrix trans, rot, scale, finTrans; 
				rot.rotate_z(shape.rotate);
				trans.translate(shape.translateX, shape.translateY, 0);
				scale.scale(shape.scaleX, shape.scaleY, 0);
				finTrans = rot * trans*scale * finTrans;
				coords = transform(finTrans.values,coords); 
				
				GLuint sVAO;
				glBufferSubData(GL_ARRAY_BUFFER, buffSize, coords.size() * sizeof(float), &coords[0]);
				buffSize = buffSize + coords.size() * sizeof(float); //tracks size of buffer offset
				glGenVertexArrays(1, &sVAO);
				glBindVertexArray(sVAO);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
				glEnableVertexAttribArray(1);

				glDrawArrays(GL_TRIANGLES, buffSize, coords.size() * sizeof(float));

				lastCoords = coords;
				vertexSize = coords.size() * sizeof(float);
				lastShape = sVAO;
			}

		}

		//Group
		else if (state == 3) {
			//No attempts worked succesfully, so i took this out
		}
		//Modify
		else if (state == 4) {
			Sleep(10);
			//Similar control concepts to viewport from lab 5
			Matrix trans, rot, scale, finTrans;
			if (isPressed(window, GLFW_KEY_R)) {
				rot.rotate_z(1);
				finTrans = rot * finTrans;
			}
			else if(isPressed(window, GLFW_KEY_UP)){
				trans.translate(0,.02,0);
				finTrans = trans* finTrans;
			}
			else if (isPressed(window, GLFW_KEY_DOWN)) {
				trans.translate(0, -.02, 0);
				finTrans = trans * finTrans;
			}
			else if (isPressed(window, GLFW_KEY_LEFT)) {
				trans.translate(-.02, 0, 0);
				finTrans = trans * finTrans;
			}
			else if (isPressed(window, GLFW_KEY_RIGHT)) {
				trans.translate(.02, 0, 0);
				finTrans = trans * finTrans;
			}
			else if (isPressed(window, GLFW_KEY_MINUS)) {
				scale.scale(1-.05, 1-.05, 1-.05);
				finTrans = scale * finTrans;
			}
			else if (isPressed(window, GLFW_KEY_EQUAL)) {
				scale.scale(1+.05, 1+0.05, 1+0.05);
				finTrans = scale * finTrans;
			}
			//change color
			else if (isPressed(window, GLFW_KEY_C)) {
				std::string data;
				std::string delimiter = ",";
				std::cout << "Enter new color (as r,g,b): " << std::endl;
				std::cin >> data;

				std::string red = data.substr(0, data.find(delimiter));
				data.erase(0, data.find(delimiter) + delimiter.length());
				std::string green = data.substr(0, data.find(delimiter));
				data.erase(0, data.find(delimiter) + delimiter.length());
				float r = atof(red.c_str());
				float g = atof(green.c_str());
				float b = atof(data.c_str());
				float newColors[] = { r,g,b };
				lastCoords = changeColor(newColors,lastCoords);
				
			}
			lastCoords = transform(finTrans.values, lastCoords);
			glBufferSubData(GL_ARRAY_BUFFER, buffSize - vertexSize, vertexSize, &lastCoords[0]);
			glDrawArrays(GL_TRIANGLES, buffSize, vertexSize);
			lastCoords = lastCoords;
		}
		//uniforms
        Uniform::set(shader.id(), "model", model);
        Uniform::set(shader.id(), "projection", projection);
        Uniform::set(shader.id(), "camera", camera);

		// render the shape
        glBindVertexArray(circleVAO);
        glDrawArrays(GL_TRIANGLES, 0, c.coords.size()*sizeof(float));
		glBindVertexArray(triVAO);
		glDrawArrays(GL_TRIANGLES, 0, tri.coords.size() * sizeof(float));

        /* Swap front and back and poll for io events */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
