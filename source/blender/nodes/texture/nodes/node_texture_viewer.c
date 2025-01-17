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

/** \file
 * \ingroup texnodes
 */

#include "NOD_texture.h"
#include "node_texture_util.h"
#include <math.h>

static bNodeSocketTemplate inputs[] = {
    {SOCK_RGBA, N_("Color"), 1.0f, 0.0f, 0.0f, 1.0f},
    {-1, ""},
};
static bNodeSocketTemplate outputs[] = {
    {-1, ""},
};

static void exec(void *data,
                 int UNUSED(thread),
                 bNode *UNUSED(node),
                 bNodeExecData *UNUSED(execdata),
                 bNodeStack **in,
                 bNodeStack **UNUSED(out))
{
  TexCallData *cdata = (TexCallData *)data;

  if (cdata->do_preview) {
    TexParams params;
    float col[4];
    params_from_cdata(&params, cdata);

    tex_input_rgba(col, in[0], &params, cdata->thread);
  }
}

void register_node_type_tex_viewer(void)
{
  static bNodeType ntype;

  tex_node_type_base(&ntype, TEX_NODE_VIEWER, "Viewer", NODE_CLASS_OUTPUT);
  node_type_socket_templates(&ntype, inputs, outputs);
  node_type_exec(&ntype, NULL, NULL, exec);

  ntype.no_muting = true;
  ntype.flag |= NODE_PREVIEW;

  nodeRegisterType(&ntype);
}
