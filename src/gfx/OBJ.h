#pragma once

#include <vector>
#include <string>
#include <ostream>
#include <set>
#include <Vector.h>

class OBJ
{
	public:

		OBJ() {}
		OBJ(const char* filename);
		~OBJ (void);

		void read(const char* filename, double scale = 1.0f);

		void write(const char* filename);
		void write(std::ostream& out);

		void smoothNormals();

		/**
		 * Returns the number of vertices that are duplicates.
		 */
		unsigned int countDuplicateVertices() const;

		void removeDuplicateVertices();

		/**
		 * Useful for geometry shader programming where info about
		 * adjacent primitives is often needed.
		 */
		void generateLineAdjacencyBuffer() const;

		const std::vector<math::vec3d>& vertices() const;
		const std::vector<math::vec2d>& texcoords() const;
		const std::vector<math::vec3d>& normals() const;

		const std::vector<unsigned int>& vertexIndices() const;
		const std::vector<unsigned int>& texcoordIndices() const;
		const std::vector<unsigned int>& normalIndices() const;

	private:

		void tokenize(const std::string& line,
					  std::vector<std::string>& tokens) const;

		void process(const std::vector<std::string>& tokens);

		void addVertex(const std::vector<std::string>& tokens);
		void addTexCoord(const std::vector<std::string>& tokens);
		void addNormal(const std::vector<std::string>& tokens);
		void addFace(const std::vector<std::string>& tokens);

		math::vec3d calculateNormal(unsigned int j);

		bool checkClose(double a, double b, double maxRelativeError) const;

		bool checkClose(const math::vec3d& a,
						const math::vec3d& b,
						double maxRelativeError = 0.001) const;

		//void considerEdge(unsigned int ia, unsigned int ib, unsigned int ic,
		//                  EdgeSet& edges) const;
		
		std::vector<math::vec3d> _vertices;
		std::vector<math::vec2d> _texcoords;
		std::vector<math::vec3d> _normals;

		std::vector<unsigned int> _vertexIndices;
		std::vector<unsigned int> _texcoordIndices;
		std::vector<unsigned int> _normalIndices;

		double _scale;

		typedef std::vector<math::vec3d>::iterator iter;
};


