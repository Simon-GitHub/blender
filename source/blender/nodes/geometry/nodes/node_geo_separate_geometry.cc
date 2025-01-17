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
 */

#include "UI_interface.h"
#include "UI_resources.h"

#include "node_geometry_util.hh"

namespace blender::nodes::node_geo_separate_geometry_cc {

NODE_STORAGE_FUNCS(NodeGeometrySeparateGeometry)

static void node_declare(NodeDeclarationBuilder &b)
{
  b.add_input<decl::Geometry>(N_("Geometry"));
  b.add_input<decl::Bool>(N_("Selection"))
      .default_value(true)
      .hide_value()
      .supports_field()
      .description(N_("The parts of the geometry that go into the first output"));
  b.add_output<decl::Geometry>(N_("Selection"))
      .description(N_("The parts of the geometry in the selection"));
  b.add_output<decl::Geometry>(N_("Inverted"))
      .description(N_("The parts of the geometry not in the selection"));
}

static void node_layout(uiLayout *layout, bContext *UNUSED(C), PointerRNA *ptr)
{
  uiItemR(layout, ptr, "domain", 0, "", ICON_NONE);
}

static void node_init(bNodeTree *UNUSED(tree), bNode *node)
{
  NodeGeometrySeparateGeometry *data = MEM_cnew<NodeGeometrySeparateGeometry>(__func__);
  data->domain = ATTR_DOMAIN_POINT;

  node->storage = data;
}

static void node_geo_exec(GeoNodeExecParams params)
{
  GeometrySet geometry_set = params.extract_input<GeometrySet>("Geometry");

  const Field<bool> selection_field = params.extract_input<Field<bool>>("Selection");

  const NodeGeometrySeparateGeometry &storage = node_storage(params.node());
  const AttributeDomain domain = static_cast<AttributeDomain>(storage.domain);

  bool all_is_error = false;
  GeometrySet second_set(geometry_set);
  if (params.output_is_required("Selection")) {
    geometry_set.modify_geometry_sets([&](GeometrySet &geometry_set) {
      bool this_is_error = false;
      separate_geometry(geometry_set,
                        domain,
                        GEO_NODE_DELETE_GEOMETRY_MODE_ALL,
                        selection_field,
                        false,
                        this_is_error);
      all_is_error &= this_is_error;
    });
    params.set_output("Selection", std::move(geometry_set));
  }
  if (params.output_is_required("Inverted")) {
    second_set.modify_geometry_sets([&](GeometrySet &geometry_set) {
      bool this_is_error = false;
      separate_geometry(geometry_set,
                        domain,
                        GEO_NODE_DELETE_GEOMETRY_MODE_ALL,
                        selection_field,
                        true,
                        this_is_error);
      all_is_error &= this_is_error;
    });
    params.set_output("Inverted", std::move(second_set));
  }
  if (all_is_error) {
    /* Only show this if none of the instances/components actually changed. */
    params.error_message_add(NodeWarningType::Info, TIP_("No geometry with given domain"));
  }
}

}  // namespace blender::nodes::node_geo_separate_geometry_cc

void register_node_type_geo_separate_geometry()
{
  namespace file_ns = blender::nodes::node_geo_separate_geometry_cc;

  static bNodeType ntype;

  geo_node_type_base(&ntype, GEO_NODE_SEPARATE_GEOMETRY, "Separate Geometry", NODE_CLASS_GEOMETRY);

  node_type_storage(&ntype,
                    "NodeGeometrySeparateGeometry",
                    node_free_standard_storage,
                    node_copy_standard_storage);

  node_type_init(&ntype, file_ns::node_init);

  ntype.declare = file_ns::node_declare;
  ntype.geometry_node_execute = file_ns::node_geo_exec;
  ntype.draw_buttons = file_ns::node_layout;
  nodeRegisterType(&ntype);
}
