#ifndef _SKY_H_
#define _SKY_H_

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <render/shader.h>
#include <render/texture.h>

#include <vector>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stb/stb_image.h>


float skyboxVertices[] =
{
	//   Coordinates
	-1.0f, -1.0f,  1.0f,//        7--------6
	 1.0f, -1.0f,  1.0f,//       /|       /|
	 1.0f, -1.0f, -1.0f,//      4--------5 |
	-1.0f, -1.0f, -1.0f,//      | |      | |
	-1.0f,  1.0f,  1.0f,//      | 3------|-2
	 1.0f,  1.0f,  1.0f,//      |/       |/
	 1.0f,  1.0f, -1.0f,//      0--------1
	-1.0f,  1.0f, -1.0f
};

unsigned int skyboxIndices[] =
{
	// Right
	1, 2, 6,
	6, 5, 1,
	// Left
	0, 4, 7,
	7, 3, 0,
	// Top
	4, 5, 6,
	6, 7, 4,
	// Bottom
	0, 3, 2,
	2, 1, 0,
	// Back
	0, 1, 5,
	5, 4, 0,
	// Front
	3, 7, 6,
	6, 2, 3
};

std::string CubemapFolder = "sky";
std::string CubeMapTextureFaces[6] =
{
	"C:/Users/Work/Downloads/assignment1/assignment1/assignment1/src/" + CubemapFolder + "/right.png",
	"C:/Users/Work/Downloads/assignment1/assignment1/assignment1/src/" + CubemapFolder + "/left.png",
	"C:/Users/Work/Downloads/assignment1/assignment1/assignment1/src/" + CubemapFolder + "/top.png",
	"C:/Users/Work/Downloads/assignment1/assignment1/assignment1/src/" + CubemapFolder + "/bottom.png",
	"C:/Users/Work/Downloads/assignment1/assignment1/assignment1/src/" + CubemapFolder + "/front.png",
	"C:/Users/Work/Downloads/assignment1/assignment1/assignment1/src/" + CubemapFolder + "/back.png"
};

struct Sky {
	

	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;

	GLuint CubeMapTexture;
	GLuint programID;

	void initialize() {
		// Create and compile our GLSL program from the shaders
		programID = LoadShaders("C:/Users/Work/Downloads/assignment1/assignment1/assignment1/src/sky.vert", "C:/Users/Work/Downloads/assignment1/assignment1/assignment1/src/sky.frag");

		if (programID == 0)
		{
			std::cerr << "Failed to load shaders." << std::endl;
		}
		
		glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &skyboxVBO);
		glGenBuffers(1, &skyboxEBO);
		glBindVertexArray(skyboxVAO);

		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glGenTextures(1, &CubeMapTexture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMapTexture);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		for (unsigned int i = 0; i < 6; i++)
		{
			int width, height, numberOfChannels;
			unsigned char* data = stbi_load(CubeMapTextureFaces[i].c_str(), &width, &height, &numberOfChannels, 0);
			if (data)
			{
				stbi_set_flip_vertically_on_load(false);
				glTexImage2D
				(
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0,
					GL_RGBA,
					width,
					height,
					0,
					GL_RGBA,
					GL_UNSIGNED_BYTE,
					data
				);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Failed to load texture: " << CubeMapTextureFaces[i] << std::endl;
				stbi_image_free(data);
			}
		}
	}

	void render(glm::mat4 cameraMatrix) {


		//Skybox
		glDepthFunc(GL_LEQUAL);
		glUniformMatrix4fv(glGetUniformLocation(programID, "ViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(cameraMatrix));

		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMapTexture);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

	}

	void cleanup() {
		glDeleteBuffers(1, &skyboxVBO);
		glDeleteBuffers(1, &skyboxEBO);
		glDeleteVertexArrays(1, &skyboxVAO);
		glDeleteTextures(1, &CubeMapTexture);
		glDeleteProgram(programID);
	}
}; 

#endif
