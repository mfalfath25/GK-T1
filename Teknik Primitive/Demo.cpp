#include "Demo.h"



Demo::Demo() {

}


Demo::~Demo() {
}



void Demo::Init() {
	// build and compile our shader program
	// ------------------------------------
	shaderProgram = BuildShader("Cube.vert", "Cube.frag", nullptr);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// position(x,y,z) ------- color(r,g,b)

		// Top base
		-0.5,  0.5, 0.5, 1.0, 0.0, 1.0,  // front top left 0
		0.5, 0.5, 0.5, 0.0, 1.0, 1.0,   // front top right 1
		0.5, 0.5, -0.5, 1.0, 1.0, 0.0,   // back top right 2
		-0.5,  0.5, -0.5, 0.0, 1.0, 1.0, // back top left 3

		// Bottom base
		-0.5, 0.4, 0.5, 1.0, 0.0, 1.0, // front bottom left 4
		0.5, 0.4, 0.5,  0.0, 1.0, 1.0, // front bottom right 5
		0.5, 0.4, -0.5, 1.0, 1.0, 0.0, // back bottom right 6
		-0.5, 0.4, -0.5, 0.0, 1.0, 1.0, // back bottom left 7

		// Top stand
		-0.5, 0.8, -0.2, 1.0, 0.0, 1.0, // front bottom left 8
		0.5, 0.8, -0.2,  0.0, 1.0, 1.0, // front bottom right 9
		0.5, 0.8, -0.5, 1.0, 1.0, 0.0, // back bottom right 10
		-0.5, 0.8, -0.5, 0.0, 1.0, 1.0, // back bottom left 11

		// Bottom stand
		-0.5, 0.5, -0.2, 1.0, 0.0, 1.0, // front bottom left 12
		0.5, 0.5, -0.2,  0.0, 1.0, 1.0, // front bottom right 13
		0.5, 0.5, -0.5, 1.0, 1.0, 0.0, // back bottom right 14
		-0.5, 0.5, -0.5, 0.0, 1.0, 1.0, // back bottom left 15
	
		// right top leg
		0.4, 0.4, 0.5, 1.0, 0.0, 1.0, // front bottom left 16
		0.5, 0.4, 0.5,  0.0, 1.0, 1.0, // front bottom right 17
		0.5, 0.4, -0.5, 1.0, 1.0, 0.0, // back bottom right 18
		0.4, 0.4, -0.5, 0.0, 1.0, 1.0, // back bottom left 19
	
		// right bottom leg
		0.4, 0.2, 0.5, 1.0, 0.0, 1.0, // front bottom left 20
		0.5, 0.2, 0.5,  0.0, 1.0, 1.0, // front bottom right 21
		0.5, 0.2, -0.5, 1.0, 1.0, 0.0, // back bottom right 22
		0.4, 0.2, -0.5, 0.0, 1.0, 1.0, // back bottom left 23

		// left top leg
		-0.5, 0.4, 0.5, 1.0, 0.0, 1.0, // front bottom left 24
		-0.4, 0.4, 0.5,  0.0, 1.0, 1.0, // front bottom right 25
		-0.4, 0.4, -0.5, 1.0, 1.0, 0.0, // back bottom right 26
		-0.5, 0.4, -0.5, 0.0, 1.0, 1.0, // back bottom left 27

		// left bottom leg
		-0.5, 0.2, 0.5, 1.0, 0.0, 1.0, // front bottom left 28
		-0.4, 0.2, 0.5,  0.0, 1.0, 1.0, // front bottom right 29
		-0.4, 0.2, -0.5, 1.0, 1.0, 0.0, // back bottom right 30
		-0.5, 0.2, -0.5, 0.0, 1.0, 1.0, // back bottom left 31
	};
	unsigned int indices[] = {  // note that we start from 0!

		// BASE
		0, 1, 2, 0, 2, 3,  // top
		4, 5, 6, 4, 6, 7,  // bottom
		4, 5, 1, 4, 1, 0,  // front face
		7, 6, 2, 7, 2, 3,  // Back Face
		5, 2, 6, 5, 1, 2,  // Right Face
		4, 7, 3, 4, 3, 0,  // Left Face

		// STAND
		8, 9, 10, 8, 10, 11,  // top
		12, 13, 14, 12, 14, 15,  // bottom
		12, 13, 9, 12, 9, 8,  // front face
		15, 14, 10, 15, 10, 11,  // Back Face
		13, 10, 14, 13, 9, 10,  // Right Face
		12, 15, 11, 12, 11, 8,  // Left Face

		// RIGHT
		16, 17, 18, 16, 18, 19,  // top
		20, 21, 22, 20, 22, 23,  // bottom
		20, 21, 17, 20, 17, 16,  // front face
		23, 22, 18, 23, 18, 19,  // Back Face
		21, 18, 22, 21, 17, 18,  // Right Face
		20, 23, 19, 20, 19, 16,  // Left Face

		// LEFT
		24, 25, 26, 24, 26, 27,  // top
		28, 29, 30, 28, 30, 31,  // bottom
		28, 29, 25, 28, 25, 24,  // front face
		31, 30, 26, 31, 26, 27,  // Back Face
		29, 26, 30, 29, 25, 26,  // Right Face
		28, 31, 27, 28, 27, 24,  // Left Face
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define color pointer layout 1
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
}

void Demo::DeInit() {
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Demo::Update(double deltaTime) {

}

void Demo::Render() {
	// render
	// ------
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);

	// draw our first triangle
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	// Pass perspective projection matrix
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(this->shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// LookAt camera (position, target/direction, up)
	glm::mat4 view = glm::lookAt(glm::vec3(0, 1, 2), glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f));
	GLint viewLoc = glGetUniformLocation(this->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	glDrawElements(GL_TRIANGLES, 1000, GL_UNSIGNED_INT, 0);
	// glBindVertexArray(0); // no need to unbind it every time
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Demo::ProcessInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main(int argc, char** argv) {
	RenderEngine &app = Demo();
	app.Start("Primitive: Draw a Cube using Triangles", 800, 600, true, false);
}