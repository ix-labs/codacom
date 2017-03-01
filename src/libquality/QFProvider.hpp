// Copyright GREYC - UMR 6072 ; Université de Caen Normandie
// Esplanade de la paix
// CS 14032
// 14032 Caen CEDEX 5
// contributeur :
// Jean Creusefond, 2015
// 
// jean.creusefond@unicaen.fr
// 
// Ce logiciel est un programme informatique servant à la comparaison et à l'analyse d'outils de détection de communauté.
// 
// Ce logiciel est régi par la licence CeCILL-B soumise au droit français et respectant les principes de diffusion des logiciels libres. Vous pouvez utiliser, modifier et/ou redistribuer ce programme sous les conditions de la licence CeCILL-B telle que diffusée par le CEA, le CNRS et l'INRIA sur le site "http://www.cecill.info".
// 
// En contrepartie de l'accessibilité au code source et des droits de copie, de modification et de redistribution accordés par cette licence, il n'est offert aux utilisateurs qu'une garantie limitée.  Pour les mêmes raisons, seule une responsabilité restreinte pèse sur l'auteur du programme, le titulaire des droits patrimoniaux et les concédants successifs.
// 
// A cet égard  l'attention de l'utilisateur est attirée sur les risques
// associés au chargement,  à l'utilisation,  à la modification et/ou au
// développement et à la reproduction du logiciel par l'utilisateur étant
// donné sa spécificité de logiciel libre, qui peut le rendre complexe à
// manipuler et qui le réserve donc à des développeurs et des professionnels avertis possédant  des  connaissances  informatiques approfondies.  Les utilisateurs sont donc invités à charger  et  tester  l'adéquation  du logiciel à leurs besoins dans des conditions permettant d'assurer la sécurité de leurs systèmes et ou de leurs données et, plus généralement, à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.
// 
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
// pris connaissance de la licence CeCILL-B, et que vous en avez accepté les termes.
// 
// ==========================================================================
// 
// Copyright GREYC - UMR 6072 ; Université de Caen Normandie
// Esplanade de la paix
// CS 14032
// 14032 Caen CEDEX 5
// contributor(s) :
// Jean Creusefond, 2015
// 
// jean.creusefond@unicaen.fr
// 
// This software is a computer program whose purpose is to compare and analyse community detection tools.
// 
// This software is governed by the CeCILL-B license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL-B
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// 
// As a counterpart to the access to the source code and  rights to copy,
// modify and redistribute granted by the license, users are provided only
// with a limited warranty  and the software's author,  the holder of the
// economic rights,  and the successive licensors  have only  limited
// liability.
// 
// In this respect, the user's attention is drawn to the risks associated
// with loading,  using,  modifying and/or developing or reproducing the
// software by the user in light of its specific status of free software,
// that may mean  that it is complicated to manipulate,  and  that  also
// therefore means  that it is reserved for developers  and  experienced
// professionals having in-depth computer knowledge. Users are therefore
// encouraged to load and test the software's suitability as regards their
// requirements in conditions enabling the security of their systems and/or
// data to be ensured and,  more generally, to use and operate it in the
// same conditions as regards security.
// 
// The fact that you are presently reading this means that you have had
// knowledge of the CeCILL-B license and that you accept its terms. 
#ifndef QF_PROVIDER
#define QF_PROVIDER

#include "quality/AbstractQF.hpp"
#include <list>

/**
 * The Quality Function (QF) provider.
 * It implements the Facade pattern, providing a set of useful functions to handle the quality functions as a bulk.
 * It handles availibility of quality functions, and provides quality functions objects accordingly
 *
 */
class QFProvider {
public:
	/**
	 * Initialisation of the QF objects
	 */
  QFProvider();
	/**
	 * Sets every QF as available
	 */
  void setAllQFAsAvailable();
	/**
	 * Sets one QF as available
	 * @param[in] name The name of the function
	 * @return 1 if the name does not correspond to any QF, else returns 0
	 */
  int setQFAsAvailable(std::string name);
	/**
	 * Sets one QF as unavailable
	 * @param[in] name The name of the function
	 * @return 1 if the name does not correspond to any QF, else returns 0
	 */
  int setQFAsUnavailable(std::string name);
	/**
	 * Gets one QF that has to be available
	 * @param[in] name The name of the function
	 * @return The QF object if the name does not correspond to any available QF, else returns NULL
	 */
  AbstractQF* getAvailableQF(std::string name);
	/**
	 * Gets all the QFs, available or not
	 * @param[out] result The QFs
	 */
  void getAllQF(std::list<AbstractQF*> & result);
	/**
	 * Gets all the available QFs
	 * @param[out] result The QFs
	 */
  void getAllAvailableQF(std::list<AbstractQF*> & result);
	/**
	 * Standard destructor
	 */
  ~QFProvider();
private:
	/**
	 * A list of pair of function/availability
	 */
  std::list<std::pair<AbstractQF*, bool> > functions;
};

#endif
