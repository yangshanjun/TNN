// Tencent is pleased to support the open source community by making TNN available.
//
// Copyright (C) 2020 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// https://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include <stdlib.h>

#include "tnn/interpreter/tnn/layer_interpreter/abstract_layer_interpreter.h"

namespace TNN_NS {

DECLARE_LAYER_INTERPRETER(Concat, LAYER_CONCAT);

Status ConcatLayerInterpreter::InterpretProto(str_arr layer_cfg_arr, int index, LayerParam** param) {
    auto p = CreateLayerParam<ConcatLayerParam>(param);
    GET_INT_1_OR_DEFAULT(p->axis, 1);
    return RPD_OK;
}

Status ConcatLayerInterpreter::InterpretResource(Deserializer& deserializer, LayerResource** resource) {
    return RPD_OK;
}

Status ConcatLayerInterpreter::SaveProto(std::ofstream& output_stream, LayerParam* param) {
    CAST_OR_RET_ERROR(layer_param, ConcatLayerParam, "invalid concat param to save", param);
    output_stream << layer_param->axis << " ";
    return RPD_OK;
}

Status ConcatLayerInterpreter::SaveResource(Serializer& serializer, LayerParam* param, LayerResource* resource) {
    return RPD_OK;
}

REGISTER_LAYER_INTERPRETER(Concat, LAYER_CONCAT);

}  // namespace TNN_NS
