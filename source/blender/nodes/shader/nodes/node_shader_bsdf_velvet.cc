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
 * The Original Code is Copyright (C) 2005 Blender Foundation.
 * All rights reserved.
 */

#include "node_shader_util.hh"

namespace blender::nodes::node_shader_bsdf_velvet_cc {

/* **************** OUTPUT ******************** */

static bNodeSocketTemplate sh_node_bsdf_velvet_in[] = {
    {SOCK_RGBA, N_("Color"), 0.8f, 0.8f, 0.8f, 1.0f, 0.0f, 1.0f},
    {SOCK_FLOAT, N_("Sigma"), 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, PROP_FACTOR},
    {SOCK_VECTOR, N_("Normal"), 0.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, PROP_NONE, SOCK_HIDE_VALUE},
    {-1, ""},
};

static bNodeSocketTemplate sh_node_bsdf_velvet_out[] = {
    {SOCK_SHADER, N_("BSDF")},
    {-1, ""},
};

static int node_shader_gpu_bsdf_velvet(GPUMaterial *mat,
                                       bNode *node,
                                       bNodeExecData *UNUSED(execdata),
                                       GPUNodeStack *in,
                                       GPUNodeStack *out)
{
  if (!in[2].link) {
    GPU_link(mat, "world_normals_get", &in[2].link);
  }

  GPU_material_flag_set(mat, GPU_MATFLAG_DIFFUSE);

  return GPU_stack_link(mat, node, "node_bsdf_velvet", in, out);
}

}  // namespace blender::nodes::node_shader_bsdf_velvet_cc

/* node type definition */
void register_node_type_sh_bsdf_velvet()
{
  namespace file_ns = blender::nodes::node_shader_bsdf_velvet_cc;

  static bNodeType ntype;

  sh_node_type_base(&ntype, SH_NODE_BSDF_VELVET, "Velvet BSDF", NODE_CLASS_SHADER);
  node_type_socket_templates(
      &ntype, file_ns::sh_node_bsdf_velvet_in, file_ns::sh_node_bsdf_velvet_out);
  node_type_gpu(&ntype, file_ns::node_shader_gpu_bsdf_velvet);

  nodeRegisterType(&ntype);
}
