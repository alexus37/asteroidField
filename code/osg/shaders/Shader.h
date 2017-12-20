/**
 * \brief Functionality for applying the shader to the object.
 *
 * \Author: Alexander Lelidis, Andreas Emch, Uroš Tešić
 * \Date:   2017-12-12
 */

#pragma once

#include <osg/Program>


namespace pbs17 {
	/**
	 * \brief Interface for the shader.
	 */
	class Shader {

	public:

		/**
		 * \brief Constructor.
		 */
		Shader();


		/**
		 * \brief Destructor.
		 */
		virtual ~Shader();


		/**
		 * \brief Apply the shader-programm to the given node.
		 * 
		 * \param node
		 *      The node which will contain the shader.
		 */
		virtual void apply(osg::Node* node) = 0;


		/**
		 * \brief Get the vertex-shader-programm.
		 * 
		 * \return The vertex-shader-programm.
		 */
		const char* getVertShader() const {
			return _vertSource;
		}


		/**
		 * \brief Set the vertex-shader-programm.
		 * 
		 * \param shader
		 *      The vertex-shader-programm.
		 */
		void setVertShader(char* shader) {
			_vertSource = shader;
		}


		/**
		 * \brief Get the fragment-shader-programm.
		 * 
		 * \return The fragment-shader-programm.
		*/
		const char* getFragShader() {
			return _fragSource;
		}


		/**
		 * \brief Set the fragment-shader-programm.
		 * 
		 * \param shader
		 *      The fragment-shader-programm.
		 */
		void setFragShader(char* shader) {
			_fragSource = shader;
		}


	private:

		//! The program for the vertex-shader.
		const char* _vertSource;

		//! The porgramm for the fragment-shader.
		const char* _fragSource;

	};
}
