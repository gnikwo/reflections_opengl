#include "stdafx.h"
#include "ObjectLoader.h"

#include <regex>

using namespace std;

ObjectLoader::ObjectLoader(std::string name): Loader("Objects/" + name + "/" + name +  ".obj", name)
{



}


ObjectLoader::~ObjectLoader()
{



}


Object* ObjectLoader::load()
{
	
	cout << "[ObjectLoader] : load" << endl;

	Object* root = new Object();

	if(ouvrir()){

		/*TODO: CHargement des materials*/

		char* code = (char*)malloc(sizeof(char) * 250);
		std::regex reg_object("^o");

		setPositionCurseur(0, DEBUT);
		while (!m__fichier->eof())
		{

			m__fichier->get(code, 200, ' ');
						
			if (std::regex_match(code, reg_object))
			{

				chargerObjet(root);

			}

			m__fichier->ignore(200, '\n');

		}

		free(code);

		fermer();

	}

	return root;

}


void ObjectLoader::chargerObjet(Object* parent)
{
	char* code = (char*) malloc(sizeof(char) * 250);
	cout << "[ObjectLoader] chargerObjet() : new object" << endl;

	Material* material;
	std::string nom;
	
	std::vector<glm::ivec3> indicesVertices;
	std::vector<glm::ivec3> indicesNormals;
	std::vector<glm::ivec3> indicesCoordTextures;

	//==Regex de detection de lignes==//
	std::regex reg_object("^o");
	std::regex reg_vertex("^v");
	std::regex reg_texture_coord("^vt");
	std::regex reg_normal("^vn");
	std::regex reg_face("^f");
	std::regex reg_material("^usemtl");

	*m__fichier >> code;

	while (!m__fichier->eof())
	{

		m__fichier->ignore(200, '\n');
		m__fichier->get(code, 10, ' ');
		cout << "[ObjectLoader] chargerObjet() : code : " << code << endl;

		if (std::regex_match(code, reg_object) ) //Si c'est un autre objet
		{

			chargerObjet(parent);

		}
		else if (std::regex_match(code, reg_vertex)) //Si c'est un vertex
		{
			std::vector<glm::vec3> temp = chargerVertice();
			m__vertices.insert(m__vertices.end(), temp.begin(), temp.end());
		}
		else if (std::regex_match(code, reg_texture_coord)) //Si c'est une coordonn� de texture
		{
			std::vector<glm::vec2> temp = chargerCoordTexture();
			m__coordTextures.insert(m__coordTextures.end(), temp.begin(), temp.end());
		}
		else if (std::regex_match(code, reg_normal)) //Si c'est une normale
		{
			std::vector<glm::vec3> temp = chargerNormale();
			m__normals.insert(m__normals.end(), temp.begin(), temp.end());
		}
		else if (std::regex_match(code, reg_material))
		{
			material = chargerMaterial();
		}
		else if (std::regex_match(code, reg_face)) //Si c'est une face
		{
			chargerFace(&indicesVertices, &indicesCoordTextures, &indicesNormals);
		}
		
	}

	free(code);

	cout << "[ObjectLoader] chargerObjet() : test_1" << endl;

	Object* obj = new Object();

	std::vector<glm::vec3> finalVertices = associerVertexIndices(m__vertices, indicesVertices);
	std::vector<glm::vec2> finalCoordTextures = associerCoordTextureFaces(m__coordTextures, indicesCoordTextures);
	std::vector<glm::vec3> finalNormals = associerNormalIndices(m__normals, indicesNormals);

	ModelRender* model = new ModelRender();
	model->setVertices(finalVertices);
	model->setUVs(finalCoordTextures);
	model->setNormals(finalNormals);
		
	Material* mat = new Material();

	model->setMaterial(mat); //model->setMaterial(material);

	obj->setRenderModel(model);

	parent->addObject(obj);

	
	/*
	FichierMTL fichierM(m_emplacementFichier, material_name);
	fichierM.ouvrir();
	fichierM.charger();


	m_objets.push_back(new ObjectData(nom));


	Material* mat = new Material(material);

	std::string nomTextureDiffuse = fichierM.getEmplacementTextureDiffuse();
	if (nomTextureDiffuse.size() > 0)
	{

		mat->setUtilisationDiffuse(true);

		std::string ext = ".png";
		unsigned int pos = nomTextureDiffuse.find(ext);
		if (pos != std::string::npos)
			nomTextureDiffuse.erase(pos, ext.length());

		mat->setTextureDiffuse(TextureLibrary::getInstance()->get(nomTextureDiffuse));

	}
	else
	{

		mat->setUtilisationDiffuse(false);

	}

	std::string nomTextureNormal = fichierM.getEmplacementTextureNormal();
	if (nomTextureNormal.size() > 0)
	{

		mat->setUtilisationNormal(true);

		std::string ext = ".png";
		unsigned int pos = nomTextureNormal.find(ext);
		if (pos != std::string::npos)
			nomTextureNormal.erase(pos, ext.length());

		mat->setTextureNormal(TextureLibrary::getInstance()->get(nomTextureNormal));

	}
	else
	{

		mat->setUtilisationNormal(false);

	}


	std::string nomTextureSpecular = fichierM.getEmplacementTextureSpecular();
	if (nomTextureSpecular.size() > 0)
	{

		mat->setUtilisationSpecular(true);

		std::string ext = ".png";
		unsigned int pos = nomTextureSpecular.find(ext);
		if (pos != std::string::npos)
			nomTextureSpecular.erase(pos, ext.length());

		mat->setTextureSpecular(TextureLibrary::getInstance()->get(nomTextureSpecular));

	}
	else
	{

		mat->setUtilisationSpecular(false);

	}


	std::string nomTextureDisplacement = fichierM.getEmplacementTextureDisplacement();
	if (nomTextureDisplacement.size() > 0)
	{

		mat->setUtilisationDisplacement(true);

		std::string ext = ".png";
		unsigned int pos = nomTextureDisplacement.find(ext);
		if (pos != std::string::npos)
			nomTextureDisplacement.erase(pos, ext.length());

		mat->setTextureDisplacement(TextureLibrary::getInstance()->get(nomTextureDisplacement));

	}
	else
	{

		mat->setUtilisationDisplacement(false);

	}

	mat->setShader(ShaderLibrary::getInstance()->get("all"));
	mat->setShaderOmbre(ShaderLibrary::getInstance()->get("light"));
	mat->setColorDiffuse(Color(fichierM.getCouleur().x, fichierM.getCouleur().y, fichierM.getCouleur().z, fichierM.getCouleur().w));


	m_objets[m_objets.size() - 1]->setMaterial(MaterialLibrary::getInstance()->add(material, mat));

	fichierM.fermer();

	setPositionCurseur(-2, CURSEUR);*/

}


std::vector<glm::vec3> ObjectLoader::chargerVertice()
{

	std::vector<glm::vec3> vertices;
	
	float x, y, z;

	*m__fichier >> x >> y >> z;
	
	vertices.push_back(glm::vec3(x, y, z));

	return vertices;
}


std::vector<glm::vec2> ObjectLoader::chargerCoordTexture()
{
	float value = 0;
	std::vector<glm::vec2> coordTexture;

	float x, y;

	*m__fichier >> x >> y;

	coordTexture.push_back(glm::vec2(x, y));

	return coordTexture;
}


std::vector<glm::vec3> ObjectLoader::chargerNormale()
{

	std::vector<glm::vec3> normales;

	float x, y, z;

	*m__fichier >> x >> y >> z;

	normales.push_back(glm::vec3(x, y, z));

	return normales;
}


void ObjectLoader::chargerFace(std::vector<glm::ivec3> *facesVertices, std::vector<glm::ivec3> *facesCoordTexture, std::vector<glm::ivec3> *facesNormals)
{
	
	unsigned int vertex_1, vertex_2, vertex_3;
	unsigned int coordTexture_1, coordTexture_2, coordTexture_3;
	unsigned int normal_1, normal_2, normal_3;

	*m__fichier >> vertex_1;
	m__fichier->ignore(200, '/');
	*m__fichier >> coordTexture_1;
	m__fichier->ignore(200, '/');
	*m__fichier >> normal_1;

	*m__fichier >> vertex_2;
	m__fichier->ignore(200, '/');
	*m__fichier >> coordTexture_2;
	m__fichier->ignore(200, '/');
	*m__fichier >> normal_2;

	*m__fichier >> vertex_3;
	m__fichier->ignore(200, '/');
	*m__fichier >> coordTexture_3;
	m__fichier->ignore(200, '/');
	*m__fichier >> normal_3;


	facesVertices->push_back(glm::ivec3(vertex_1, vertex_2, vertex_3));
	facesCoordTexture->push_back(glm::ivec3(coordTexture_1, coordTexture_2, coordTexture_3));
	facesNormals->push_back(glm::ivec3(normal_1, normal_2, normal_3));
		
}


std::vector<glm::vec3> ObjectLoader::associerVertexIndices(std::vector<glm::vec3> vertices, std::vector<glm::ivec3> indices)
{

	std::vector<glm::vec3> finalVertices;

	for (unsigned int i = 0; i < indices.size(); i++)
	{

		for (unsigned int j = 0; j < 3; j++)
		{

			finalVertices.push_back(vertices[indices[i][j]-1]);

		}

	}
	return finalVertices;
}


std::vector<glm::vec2> ObjectLoader::associerCoordTextureFaces(std::vector<glm::vec2> coordTextures, std::vector<glm::ivec3> indices)
{

	std::vector<glm::vec2> finalCoordTextures;

	for (unsigned int i = 0; i < indices.size(); i++)
	{

		for (unsigned int j = 0; j < 3; j++)
		{

			finalCoordTextures.push_back(coordTextures[indices[i][j]-1]);

		}

	}
	return finalCoordTextures;
}


std::vector<glm::vec3> ObjectLoader::associerNormalIndices(std::vector<glm::vec3> normals, std::vector<glm::ivec3> indices)
{

	std::vector<glm::vec3> finalNormals;

	for (unsigned int i = 0; i < indices.size(); i++)
	{

		for (unsigned int j = 0; j < 3; j++)
		{

			finalNormals.push_back(normals[indices[i][j]-1]);

		}

	}
	return finalNormals;
}


Material* ObjectLoader::chargerMaterial()
{

	string matName;

	*m__fichier >> matName;

	return m__materials[matName];;

}
