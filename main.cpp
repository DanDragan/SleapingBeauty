//-------------------------------------------------------------------------------------------------
// Descriere: fisier main
//
// Autor: Dan Dragan-Stefan 333CA
//-------------------------------------------------------------------------------------------------

#include "lab_mesh_loader.hpp"
#include "lab_geometry.hpp"
//incarcator de shadere
#include "lab_shader_loader.hpp"
#include "lab_glut.hpp"
//texturi
#include "lab_texture_loader.hpp"
#include <ctime>

// functie care intoarce o valoare reala intr-un anumit interval
float random_float(float min, float max) {
   float random = ((float) rand()) / (float) RAND_MAX;
   float range = max - min;  
   
   return (random*range) + min;
}

class Laborator : public lab::glut::WindowListener{
	
	// structura folosita pentru a retine informatii legate de un vertex
	struct VertexFormat {
		float position_x, position_y, position_z;				
		float texcoord_x, texcoord_y;

		VertexFormat(float px, float py, float pz, float tex_x, float tex_y) {
			position_x = px;		position_y = py;		position_z = pz;
			texcoord_x = tex_x;		texcoord_y = tex_y;
		}
	};

	// clasa ce contine informatii legate de o particula din sistemul de particule
	class Particle {
	public:
		std::vector<VertexFormat> vertices;
		glm::vec3 speed;
		glm::vec3 color;
		unsigned int vbo, ibo, vao, num_indices;
		float dx, dy, dz;

		Particle(std::vector<VertexFormat> vertices, glm::vec3 color, glm::vec3 speed) {
			this->vertices = vertices;
			this->speed = speed;
			this->color = color;
			num_indices = 6;
		}
	};

//variabile
private:
	glm::mat4 model_matrix, view_matrix, projection_matrix;								//matrici 4x4 pt modelare vizualizare proiectie
	unsigned int gl_program_shader_gouraud;												//id obiect shader gouraud
	unsigned int gl_program_shader_phong;												//id obiect shader phong
	unsigned int gl_program_shader_curent;												//id obiect shader curent

	unsigned int mesh_vbo_sleep, mesh_ibo_sleep, mesh_vao_sleep, mesh_num_indices_sleep;		//containere opengl pentru vertecsi, indecsi si stare
	unsigned int mesh_vbo_surprise, mesh_ibo_surprise, mesh_vao_surprise, mesh_num_indices_surprise;
	unsigned int mesh_vbo_annoyed, mesh_ibo_annoyed, mesh_vao_annoyed, mesh_num_indices_annoyed;
	
	glm::vec3 light_position;
	glm::vec3 eye_position;
	unsigned int material_shininess;
	float material_kd;
	float material_ks;
	int counter;
	int fcounter;
	int cnt;

	std::vector<Particle> particles;
	int nr_particles;

	unsigned int girl_texture;
	unsigned int bilb_texture;

	clock_t begin_time;

//metode
public:
	
	//constructor .. e apelat cand e instantiata clasa
	Laborator(){
		//setari pentru desenare, clear color seteaza culoarea de clear pentru ecran (format R,G,B,A)
		glClearColor(0.5,0.5,0.5,1);
		glClearDepth(1);			//clear depth si depth test (nu le studiem momentan, dar avem nevoie de ele!)
		glEnable(GL_DEPTH_TEST);	//sunt folosite pentru a determina obiectele cele mai apropiate de camera (la curs: algoritmul pictorului, algoritmul zbuffer)
		
		//incarca un shader din fisiere si gaseste locatiile matricilor relativ la programul creat
		gl_program_shader_gouraud = lab::loadShader("shadere\\shader_gouraud_vertex.glsl", "shadere\\shader_gouraud_fragment.glsl");
		gl_program_shader_phong = lab::loadShader("shadere\\shader_phong_vertex.glsl", "shadere\\shader_phong_fragment.glsl");
		gl_program_shader_curent = gl_program_shader_phong;
		
		lab::initialize("resurse\\girl_sleep.obj");
		lab::initialize("resurse\\girl_surprise.obj");
		lab::initialize("resurse\\girl_annoyed.obj");
		//incarca un sleep_face
		lab::loadObj("resurse\\girl_sleep.obj",mesh_vao_sleep, mesh_vbo_sleep, mesh_ibo_sleep, mesh_num_indices_sleep);	
		girl_texture = lab::loadTextureBMP("resurse\\girl_texture.bmp");
		//incarca un suprise_face
		lab::loadObj("resurse\\girl_surprise.obj",mesh_vao_surprise, mesh_vbo_surprise, mesh_ibo_surprise, mesh_num_indices_surprise);
		
		//incarca un annoyed_face
		lab::loadObj("resurse\\girl_annoyed.obj",mesh_vao_annoyed, mesh_vbo_annoyed, mesh_ibo_annoyed, mesh_num_indices_annoyed);
		
		bilb_texture = lab::loadTextureBMP("resurse\\music.bmp");

		//lumina & material
		eye_position = glm::vec3(-1,8,50);
		light_position = glm::vec3(-1,11,49);
		material_shininess = 100;
		material_kd = 0.5;
		material_ks = 0.5;
		counter = 0;
		fcounter = 0;
		cnt = 0;
		nr_particles = 1000;
		begin_time = clock();
		
		//matrici de modelare si vizualizare
		model_matrix = glm::mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
		view_matrix = glm::lookAt(eye_position, glm::vec3(0,0,0), glm::vec3(0,1,0));

		unsigned int indices[] = {0, 1, 2, 1, 3, 2};
		// initializeaza fiecare particula din sistemul de particule
		for(int i = 0; i < nr_particles; i++) {
			srand(i * time(0));

			std::vector<VertexFormat> vertices;
			float dx = random_float(-250, -80);
			float dy = random_float(-20, 30);
			float dz = 0;
			vertices.push_back(VertexFormat(-3, -3, 0, 0, 0));
			vertices.push_back(VertexFormat(3, -3, 0, 1, 0));
			vertices.push_back(VertexFormat(-3, 3, 0, 0, 1));
			vertices.push_back(VertexFormat(3, 3, 0, 1, 1));
			float color_x = (float)(rand() % 256) / 255;
			float color_y = (float)(rand() % 256) / 255;
			float color_z = (float)(rand() % 256) / 255;
			float speed_x = random_float(0.006, 0.009);
			float speed_y = random_float(-0.002, 0);
			float speed_z = 0;
			Particle part(vertices, glm::vec3(color_x, color_y, color_z), glm::vec3(speed_x, speed_y, speed_z));
			particles.push_back(part);
			particles[i].dx = dx;
			particles[i].dy = dy;
			particles[i].dz = dz;
				
			//desenare wireframe
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			glGenVertexArrays(1, &particles[i].vao);
			glBindVertexArray(particles[i].vao);
		
			//vertex buffer object -> un obiect in care tinem vertecsii
			glGenBuffers(1,&particles[i].vbo);
			glBindBuffer(GL_ARRAY_BUFFER, particles[i].vbo);
			glBufferData(GL_ARRAY_BUFFER, particles[i].vertices.size()*sizeof(VertexFormat), &particles[i].vertices[0], GL_STATIC_DRAW);

			//index buffer object -> un obiect in care tinem indecsii
			glGenBuffers(1,&particles[i].ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, particles[i].ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * sizeof(unsigned int), indices, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(VertexFormat),(void*)0);						//trimite pozitii pe pipe 0
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(VertexFormat),(void*)(sizeof(float)*3));	   //trimite texcoords pe pipe 2
		}
	}

	//destructor .. e apelat cand e distrusa clasa
	~Laborator(){
		//distruge shadere
		glDeleteProgram(gl_program_shader_gouraud);
		glDeleteProgram(gl_program_shader_phong);

		//distruge mesh incarcat
		glDeleteBuffers(1,&mesh_vbo_sleep);
		glDeleteBuffers(1,&mesh_ibo_sleep);
		glDeleteVertexArrays(1,&mesh_vbo_sleep);

		glDeleteBuffers(1,&mesh_vbo_surprise);
		glDeleteBuffers(1,&mesh_ibo_surprise);
		glDeleteVertexArrays(1,&mesh_vbo_surprise);

		glDeleteBuffers(1,&mesh_vbo_annoyed);
		glDeleteBuffers(1,&mesh_ibo_annoyed);
		glDeleteVertexArrays(1,&mesh_vbo_annoyed);
	}


	//--------------------------------------------------------------------------------------------
	//functii de cadru ---------------------------------------------------------------------------

	//functie chemata inainte de a incepe cadrul de desenare, o folosim ca sa updatam situatia scenei ( modelam/simulam scena)
	void notifyBeginFrame(){
				
	}
	//functia de afisare (lucram cu banda grafica)
	void notifyDisplayFrame(){
		//bufferele din framebuffer sunt aduse la valorile initiale (setate de clear color si cleardepth)
		//adica se sterge ecranul si se pune culoare (si alte propietati) initiala
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// durata unei translatii
		fcounter = 5000;
		// contor pentru o translatie
		counter = (clock() - begin_time) % fcounter;
		// contor pentru toate cele 4 translatii
		cnt = (clock() - begin_time) % (4*fcounter);
			
		//foloseste shaderul
		glUseProgram(gl_program_shader_curent);
				
		//trimite variabile uniforme la shader
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader_curent, "model_matrix"),1,false,glm::value_ptr(model_matrix));
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader_curent, "view_matrix"),1,false,glm::value_ptr(view_matrix));
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader_curent, "projection_matrix"),1,false,glm::value_ptr(projection_matrix));
		glUniform3f(glGetUniformLocation(gl_program_shader_curent, "light_position"),light_position.x, light_position.y, light_position.z);
		glUniform3f(glGetUniformLocation(gl_program_shader_curent, "eye_position"),eye_position.x, eye_position.y, eye_position.z);
		glUniform1i(glGetUniformLocation(gl_program_shader_curent, "material_shininess"),material_shininess);
		glUniform1f(glGetUniformLocation(gl_program_shader_curent, "material_kd"),material_kd);
		glUniform1f(glGetUniformLocation(gl_program_shader_curent, "material_ks"),material_ks);
		
		//textura nota muzicala
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, bilb_texture);
		glUniform1i(glGetUniformLocation(gl_program_shader_curent, "textura2"), 1);
		glUniform1i(glGetUniformLocation(gl_program_shader_curent, "counter"), cnt * 2.5);
		glUniform1i(glGetUniformLocation(gl_program_shader_curent, "part"), 2);
		//trimitere caracteristici particula
		for(int i = 0; i < nr_particles; i++) {
			glm::mat4 matrice_translatie = glm::translate(model_matrix, glm::vec3(particles[i].dx,particles[i].dy,particles[i].dz));
			glUniformMatrix4fv(glGetUniformLocation(gl_program_shader_curent, "model_matrix"),1,false,glm::value_ptr(matrice_translatie));
			glUniform3f(glGetUniformLocation(gl_program_shader_curent, "part_color"),particles[i].color.x, particles[i].color.y, particles[i].color.z);
			glUniform3f(glGetUniformLocation(gl_program_shader_curent, "speed"),particles[i].speed.x, particles[i].speed.y, particles[i].speed.z);
			glBindVertexArray(particles[i].vao);
			glDrawElements(GL_TRIANGLES, particles[i].num_indices, GL_UNSIGNED_INT, 0);
		}
		//bind obiect
		glm::mat4 matrice_translatie = glm::translate(model_matrix, glm::vec3(0,-40,28));
		glm::mat4 matrice_scalare = glm::scale(model_matrix, glm::vec3(7,7,7));
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader_curent, "model_matrix"),1,false,glm::value_ptr(matrice_translatie * matrice_scalare));
		
		// textura fetei
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, girl_texture);
		glUniform1i(glGetUniformLocation(gl_program_shader_curent, "textura1"), 0);
		glUniform1i(glGetUniformLocation(gl_program_shader_curent, "part"),1);
		glUniform1i(glGetUniformLocation(gl_program_shader_curent, "fcounter"),fcounter);

		// translatie sleep-surprised
		if(cnt >= 0 && cnt < 5000) {
			glUniform1i(glGetUniformLocation(gl_program_shader_curent, "counter"),counter);
			glBindVertexArray(mesh_vao_sleep);
			glDrawElements(GL_TRIANGLES, mesh_num_indices_sleep, GL_UNSIGNED_INT, 0);
		}
		// translatie surprised-annoyed
		if(cnt >= 5000 && cnt < 10000) {
			glUniform1i(glGetUniformLocation(gl_program_shader_curent, "counter"),counter);
			glBindVertexArray(mesh_vao_surprise);
			glDrawElements(GL_TRIANGLES, mesh_num_indices_surprise, GL_UNSIGNED_INT, 0);
		}
		// translatie annoyed-sleep
		if(cnt >= 10000 && cnt < 15000) {
			glUniform1i(glGetUniformLocation(gl_program_shader_curent, "counter"), counter);
			glBindVertexArray(mesh_vao_annoyed);
			glDrawElements(GL_TRIANGLES, mesh_num_indices_annoyed, GL_UNSIGNED_INT, 0);
		}
		// o noua faza sleep-sleep
		if(cnt >= 15000 && cnt < 25000) {
			glUniform1i(glGetUniformLocation(gl_program_shader_curent, "part"), 3);
			glBindVertexArray(mesh_vao_sleep);
			glDrawElements(GL_TRIANGLES, mesh_num_indices_sleep, GL_UNSIGNED_INT, 0);
		}
	}
	//functie chemata dupa ce am terminat cadrul de desenare (poate fi folosita pt modelare/simulare)
	void notifyEndFrame(){}
	//functei care e chemata cand se schimba dimensiunea ferestrei initiale
	void notifyReshape(int width, int height, int previos_width, int previous_height){
		//reshape
		if(height==0) height=1;
		glViewport(0,0,width,height);
		projection_matrix = glm::perspective(90.0f, (float)width/(float)height,0.1f, 10000.0f);
	}


	//--------------------------------------------------------------------------------------------
	//functii de input output --------------------------------------------------------------------
	
	//tasta apasata
	void notifyKeyPressed(unsigned char key_pressed, int mouse_x, int mouse_y){
		if(key_pressed == 27) lab::glut::close();	//ESC inchide glut si 
		if(key_pressed == 32) {
			//SPACE reincarca shaderul si recalculeaza locatiile (offseti/pointeri)
			glDeleteProgram(gl_program_shader_gouraud);
			glDeleteProgram(gl_program_shader_phong);
			gl_program_shader_gouraud = lab::loadShader("shadere\\shader_gouraud_vertex.glsl", "shadere\\shader_gouraud_fragment.glsl");
			gl_program_shader_phong = lab::loadShader("shadere\\shader_phong_vertex.glsl", "shadere\\shader_phong_fragment.glsl");
			gl_program_shader_curent = gl_program_shader_gouraud;
		}
		if(key_pressed == 'w'){
			static bool wire =true;
			wire=!wire;
			glPolygonMode(GL_FRONT_AND_BACK, (wire?GL_LINE:GL_FILL));
		}
		
		if(key_pressed == '2'){
			gl_program_shader_curent = gl_program_shader_phong;
		}
	}
	//tasta ridicata
	void notifyKeyReleased(unsigned char key_released, int mouse_x, int mouse_y){	}
	//tasta speciala (up/down/F1/F2..) apasata
	void notifySpecialKeyPressed(int key_pressed, int mouse_x, int mouse_y){
		if(key_pressed == GLUT_KEY_F1) lab::glut::enterFullscreen();
		if(key_pressed == GLUT_KEY_F2) lab::glut::exitFullscreen();
	}
	//tasta speciala ridicata
	void notifySpecialKeyReleased(int key_released, int mouse_x, int mouse_y){}
	//drag cu mouse-ul
	void notifyMouseDrag(int mouse_x, int mouse_y){ }
	//am miscat mouseul (fara sa apas vreun buton)
	void notifyMouseMove(int mouse_x, int mouse_y){ }
	//am apasat pe un boton
	void notifyMouseClick(int button, int state, int mouse_x, int mouse_y){ }
	//scroll cu mouse-ul
	void notifyMouseScroll(int wheel, int direction, int mouse_x, int mouse_y){ std::cout<<"Mouse scroll"<<std::endl;}

};

int main(){
	//initializeaza GLUT (fereastra + input + context OpenGL)
	lab::glut::WindowInfo window(std::string("Sleaping Beauty"),800,600,100,100,true);
	lab::glut::ContextInfo context(3,3,false);
	lab::glut::FramebufferInfo framebuffer(true,true,true,true);
	lab::glut::init(window,context, framebuffer);

	//initializeaza GLEW (ne incarca functiile openGL, altfel ar trebui sa facem asta manual!)
	glewExperimental = true;
	glewInit();
	std::cout<<"GLEW:initializare"<<std::endl;

	//creem clasa noastra si o punem sa asculte evenimentele de la GLUT
	//DUPA GLEW!!! ca sa avem functiile de OpenGL incarcate inainte sa ii fie apelat constructorul (care creeaza obiecte OpenGL)
	Laborator mylab;
	lab::glut::setListener(&mylab);

	//taste
	std::cout<<"Taste:"<<std::endl<<"\tESC ... iesire"<<std::endl<<"\tSPACE ... reincarca shadere"<<std::endl<<"\tw ... toggle wireframe"<<std::endl;
	std::cout<<"\t1 ... foloseste shader Gouraud"<<std::endl<<"\t2 ... foloseste shader Phong"<<std::endl;

	//run
	lab::glut::run();

	return 0;
}