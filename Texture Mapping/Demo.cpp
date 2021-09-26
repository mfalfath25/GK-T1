#include "Demo.h"



Demo::Demo() {

}


Demo::~Demo() {
}



void Demo::Init() {
	// build and compile our shader program
	// ------------------------------------
	shaderProgram = BuildShader("vertexShader.vert", "fragmentShader.frag", nullptr);

	BuildColoredCube();

	BuildColoredPlane();
}

void Demo::DeInit() {
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO2);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Demo::ProcessInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void Demo::Update(double deltaTime) {

}

void Demo::Render() {
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);

	// Pass perspective projection matrix
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(this->shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// LookAt camera (position, target/direction, up)
	glm::mat4 view = glm::lookAt(glm::vec3(0, 1, 2), glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f));
	GLint viewLoc = glGetUniformLocation(this->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	DrawColoredCube();

	DrawColoredPlane();

	glDisable(GL_DEPTH_TEST);
}

void Demo::BuildColoredCube() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("crate.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		
		// Top base
		-0.5,  0.5, 0.5, 0, 1,  // front top left 0
		0.5, 0.5, 0.5, 1, 1, // front top right 1
		0.5, 0.5, -0.5, 1, 1, // back top right 2
		-0.5,  0.5, -0.5, 0, 1, // back top left 3

		// Bottom base
		-0.5, 0.4, 0.5, 0, 0, // front bottom left 4
		0.5, 0.4, 0.5, 1, 0, // front bottom right 5
		0.5, 0.4, -0.5, 1, 0, // back bottom right 6
		-0.5, 0.4, -0.5, 0, 0, // back bottom left 7

		// Top stand
		-0.5, 0.8, -0.2, 0, 1, // front bottom left 8
		0.5, 0.8, -0.2, 1, 1, // front bottom right 9
		0.5, 0.8, -0.5, 1, 1, // back bottom right 10
		-0.5, 0.8, -0.5, 0, 1, // back bottom left 11

		// Bottom stand
		-0.5, 0.5, -0.2, 0, 0, // front bottom left 12
		0.5, 0.5, -0.2, 1, 0, // front bottom right 13
		0.5, 0.5, -0.5, 1, 0, // back bottom right 14
		-0.5, 0.5, -0.5, 0, 0, // back bottom left 15

		// right top leg
		0.4, 0.4, 0.5, 0, 1, // front bottom left 16
		0.5, 0.4, 0.5, 1, 1, // front bottom right 17
		0.5, 0.4, -0.5, 1, 1, // back bottom right 18
		0.4, 0.4, -0.5, 0, 1, // back bottom left 19

		// right bottom leg
		0.4, 0.2, 0.5, 0, 0, // front bottom left 20
		0.5, 0.2, 0.5, 1, 0, // front bottom right 21
		0.5, 0.2, -0.5, 1, 0, // back bottom right 22
		0.4, 0.2, -0.5, 0, 0, // back bottom left 23

		// left top leg
		-0.5, 0.4, 0.5, 0, 1, // front bottom left 24
		-0.4, 0.4, 0.5, 1, 1, // front bottom right 25
		-0.4, 0.4, -0.5, 1, 1, // back bottom right 26
		-0.5, 0.4, -0.5, 0, 1, // back bottom left 27

		// left bottom leg
		-0.5, 0.2, 0.5, 0, 0, // front bottom left 28
		-0.4, 0.2, 0.5, 1, 0, // front bottom right 29
		-0.4, 0.2, -0.5, 1, 0, // back bottom right 30
		-0.5, 0.2, -0.5, 0, 0,// back bottom left 31
	};

	unsigned int indices[] = {
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawColoredCube()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 1000, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildColoredPlane()
{
	// Load and create a texture 
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("marble.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		-50.0, -0.5, -50.0,  0,  0,
		 50.0, -0.5, -50.0, 50,  0,
		 50.0, -0.5,  50.0, 50, 50,
		-50.0, -0.5,  50.0,  0, 50,


	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}



void Demo::DrawColoredPlane()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

int main(int argc, char** argv) {
	RenderEngine &app = Demo();
	app.Start("Texture Mapping: Draw textured cube and plane", 1000, 800, true, false);
}