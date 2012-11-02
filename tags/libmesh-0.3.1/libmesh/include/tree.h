// $Id: tree.h,v 1.6 2003-02-13 22:56:08 benkirk Exp $

// The Next Great Finite Element Library.
// Copyright (C) 2002  Benjamin S. Kirk, John W. Peterson
  
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
  
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
  
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA



#ifndef __tree_h__
#define __tree_h__

// C++ includes

// Local includes
#include "tree_node.h"


/**
 * This class defines a tree.
 *
 * @author Benjamin S. Kirk, 2002
 */

// ------------------------------------------------------------
// Tree class definition
template <unsigned int N>
class Tree
{
public:
  
  /**
   * Constructor.  Does nothing at the moment.
   */
  Tree (const MeshBase& m, const unsigned int level);

  /**
   * Copy-constructor.
   */
  Tree (const Tree<N>& other_tree);

  /**
   * Destructor.
   */
  ~Tree() {}

  /**
   * Prints the nodes.
   */
  void print_nodes() const { std::cout << "Printing nodes...\n"; root.print_nodes(); }

  /**
   * Prints the nodes.
   */
  void print_elements() const { std::cout << "Printing elements...\n"; root.print_elements(); }
  

  /**
   * @returns the number of active bins.
   */
  unsigned int n_active_bins() const { return root.n_active_bins(); }

  /**
   * @returns a pointer to the element containing point p.
   */
  Elem* find_element(const Point& p) const;

private:

  /**
   * Constant reference to a mesh.  Declared
   * at construction.
   */
  const MeshBase& mesh;

  /**
   * The tree root.
   */
  TreeNode<N> root;
  
};

/**
 * For convenience we define QuadTrees and OctTrees 
 * explicitly.
 */
namespace Trees
{
  /**
   * A QuadTree is a tree appropriate
   * for 2D meshes.
   */
  typedef Tree<4> QuadTree;

  /**
   * An OctTree is a tree appropriate
   * for 3D meshes.
   */
  typedef Tree<8> OctTree;
}



// ------------------------------------------------------------
// Tree class inline methods

// constructor
template <unsigned int N>
inline
Tree<N>::Tree (const MeshBase& m, const unsigned int level) :
  mesh(m),
  root(m,level)
{
  assert (mesh.n_nodes());

  // Set the root node bounding box equal to the bounding
  // box for the entire domain.
  root.set_bounding_box (mesh.bounding_box());

  // Add all the nodal indices to the root node.  It will 
  // automagically build the tree for us.
  for (unsigned int n=0; n<mesh.n_nodes(); ++n)
    root.insert (n);

  // Now the tree contains the global node numbers.
  // However, we want element pointers, so here we
  // convert between the two.
  std::vector<std::vector<unsigned int> > nodes_to_elem;

  mesh.build_nodes_to_elem_map (nodes_to_elem);

  root.transform_nodes_to_elements (nodes_to_elem);
}



// copy-constructor
template <unsigned int N>
inline
Tree<N>::Tree (const Tree<N>& other_tree) :
  mesh(other_tree.mesh),
  root(other_tree.root)
{
  error();
}

#endif