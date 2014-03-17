EGC - Tema 4

Sleeping Beauty
333CA Dragan Dan-Stefan

1.Cerinta
2.Implementare
3.Testare
4.Continutul Arhivei
5.Functionalitati

1. Cerinta
	Realizarea unei animatii a unei fete, ce se va afla in 3 stari:
	sleep, surprised, annoyed, precum si a unui sistem de particule
	(note muzicale), ce se vor sincroniza cu animatia.
	
2. Implementare
	Tema este implementata plecand de la scheletul de cod al laboratorului
	de texturare.
	Pentru inceput am incarcat cele 3 fisiere obj cu starile fetei, modificand functia
	de incarcare. Astfel mi-am creat 3 vectori de vertecsi pentru fiecare imagine, fiecare
	vector continand si pozitiile vertecsilor imaginii urmatoare, precum si normalele aferente
	acesteia. Animatia este reprezentata de o interpolare liniara ce ruleaza pe vertex_shader.
	Am pus si textura peste cele 3 obiecte si am folosit modelul de iluminare Phong.
	Pentru partea a 2-a mi-am desenat un patrat centrat in origine pentru fiecare particula,
	care va fi translatat, la o pozitie aleatoare.Peste el am pus textura notei muzicale si
	am aplicat alfa-discard pentru eliminarea background-ului alb. Miscarea particulelor
	se realizeaza in vertex_shader, adaugand la pozitia initiala o viteza inmultita cu un contor
	ce se reseteaza la finalul animatiei complete.	
	
3. Testare
	Tema a fost testata pe un sistem cu procesor i3 dual-core 2,1GH, 3 GB memorie RAM,
    placa video NVIDIA GeForce GT 525M de 1GB, pe sistemul de operare Windows 7.
	
4. Continutul Arhivei
	-main.cpp
	-fisier resurse, ce contine fisiere .obj puse la dispozitie
	-lab.glut.hpp
	-lab.geometry.hpp
	-lab.mesh_loader.hpp
	-lab.shader_loader.hpp
	-shader_gourau_fragment.glsl
	-shader_phong_vertex.glsl
	-shader_gourau_vertex.glsl
	-shader_phong_fragment.glsl
	
5. Functionalitati
	- animatie faciala realista (texturare + iluminare)
	- sistem de particule (note muzicale)
	- sincronizare animatie cu sistem de particule