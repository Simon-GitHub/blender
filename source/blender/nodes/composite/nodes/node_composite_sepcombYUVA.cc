/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2006 Blender Foundation.
 * All rights reserved.
 */

/** \file
 * \ingroup cmpnodes
 */

#include "node_composite_util.hh"

/* **************** SEPARATE YUVA ******************** */

namespace blender::nodes {

static void cmp_node_sepyuva_declare(NodeDeclarationBuilder &b)
{
  b.add_input<decl::Color>(N_("Image")).default_value({1.0f, 1.0f, 1.0f, 1.0f});
  b.add_output<decl::Float>(N_("Y"));
  b.add_output<decl::Float>(N_("U"));
  b.add_output<decl::Float>(N_("V"));
  b.add_output<decl::Float>(N_("A"));
}

}  // namespace blender::nodes

void register_node_type_cmp_sepyuva()
{
  static bNodeType ntype;

  cmp_node_type_base(&ntype, CMP_NODE_SEPYUVA, "Separate YUVA", NODE_CLASS_CONVERTER);
  ntype.declare = blender::nodes::cmp_node_sepyuva_declare;

  nodeRegisterType(&ntype);
}

/* **************** COMBINE YUVA ******************** */

namespace blender::nodes {

static void cmp_node_combyuva_declare(NodeDeclarationBuilder &b)
{
  b.add_input<decl::Float>(N_("Y")).min(0.0f).max(1.0f);
  b.add_input<decl::Float>(N_("U")).min(0.0f).max(1.0f);
  b.add_input<decl::Float>(N_("V")).min(0.0f).max(1.0f);
  b.add_input<decl::Float>(N_("A")).default_value(1.0f).min(0.0f).max(1.0f);
  b.add_output<decl::Color>(N_("Image"));
}

}  // namespace blender::nodes

void register_node_type_cmp_combyuva()
{
  static bNodeType ntype;

  cmp_node_type_base(&ntype, CMP_NODE_COMBYUVA, "Combine YUVA", NODE_CLASS_CONVERTER);
  ntype.declare = blender::nodes::cmp_node_combyuva_declare;

  nodeRegisterType(&ntype);
}
