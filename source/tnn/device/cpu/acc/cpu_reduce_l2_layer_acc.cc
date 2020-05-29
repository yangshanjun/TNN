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

#include "tnn/utils/naive_compute.h"
#include "tnn/device/cpu/acc/cpu_reduce_layer_acc.h"
#include "tnn/utils/data_type_utils.h"
#include "tnn/utils/dims_vector_utils.h"

namespace TNN_NS {

DECLARE_CPU_REDUCE_ACC(ReduceL2, LAYER_REDUCE_L2);

Status CpuReduceL2LayerAcc::CalculateReduce(float* output_data, float* input_data, int outer_dim, int channels,
                                            int inner_dim) {
    float* origin_output_data = output_data;
    int output_size           = outer_dim * inner_dim;
    for (int oc = 0; oc < outer_dim; oc++) {
        for (int c = 0; c < channels; c++) {
            for (int ic = 0; ic < inner_dim; ic++) {
                output_data[ic] += std::pow(input_data[ic], 2);
            }
            input_data += inner_dim;
        }
        output_data += inner_dim;
    }
    for (int i = 0; i < output_size; ++i) {
        origin_output_data[i] = std::sqrt(origin_output_data[i]);
    }
    return RPD_OK;
}

REGISTER_CPU_REDUCE_ACC(ReduceL2, LAYER_REDUCE_L2);

}  // namespace TNN_NS
