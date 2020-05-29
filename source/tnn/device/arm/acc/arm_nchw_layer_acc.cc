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

#include "tnn/device/arm/acc/arm_nchw_layer_acc.h"

#include "tnn/device/arm/arm_common.h"
#include "tnn/utils/data_type_utils.h"
#include "tnn/utils/dims_vector_utils.h"

namespace TNN_NS {

ArmNchwLayerAcc::~ArmNchwLayerAcc(){};

Status ArmNchwLayerAcc::UnPackInputs(const std::vector<Blob *> &inputs) {
    for (int i = 0; i < inputs.size(); i++) {
        auto input_dims = inputs[i]->GetBlobDesc().dims;
        for (int n = 0; n < input_dims[0]; ++n) {
            auto in_count     = input_dims[3] * input_dims[2] * ROUND_UP(input_dims[1], 4);
            auto out_count    = input_dims[3] * input_dims[2] * input_dims[1];
            float *src  = reinterpret_cast<float *>(GetBlobHandlePtr(inputs[i]->GetHandle())) + n * in_count;
            float *dst = reinterpret_cast<float *>(GetBlobHandlePtr(nchw_blob_in[i]->GetHandle())) + n * out_count;
            UnpackC4(dst, src, input_dims[3] * input_dims[2], input_dims[1]);
        }
    }
    return RPD_OK;
}
Status ArmNchwLayerAcc::PackOutputs(const std::vector<Blob *> &outputs) {
    for (int i = 0; i < outputs.size(); i++) {
        auto out_dims = outputs[i]->GetBlobDesc().dims;
        for (int n = 0; n < out_dims[0]; ++n) {
            auto in_count  = out_dims[3] * out_dims[2] * out_dims[1];
            auto out_count = out_dims[3] * out_dims[2] * ROUND_UP(out_dims[1], 4);
            float *src     = reinterpret_cast<float *>(GetBlobHandlePtr(nchw_blob_out[i]->GetHandle())) + n * in_count;
            float *dst     = reinterpret_cast<float *>(GetBlobHandlePtr(outputs[i]->GetHandle())) + n * out_count;
            PackC4(dst, src, out_dims[3] * out_dims[2], out_dims[1]);
        }
    }
    return RPD_OK;
}

Status ArmNchwLayerAcc::Reshape(const std::vector<Blob *> &inputs, const std::vector<Blob *> &outputs) {
    ArmLayerAcc::Reshape(inputs, outputs);
    return RPD_OK;
}

Status ArmNchwLayerAcc::AllocConvertBuffer(const std::vector<Blob *> &inputs, const std::vector<Blob *> &outputs) {
    int space_id = 0;
    nchw_blob_in.clear();
    nchw_blob_out.clear();
    for (auto blob : inputs) {
        auto desc = blob->GetBlobDesc();
        BlobHandle handle;
        handle.base = context_->GetSharedWorkSpace(
            DimsVectorUtils::Count(desc.dims) * DataTypeUtils::GetBytesSize(desc.data_type), space_id++);
        nchw_blob_in.push_back(std::make_shared<Blob>(desc, handle));
    }

    for (auto blob : outputs) {
        auto desc = blob->GetBlobDesc();
        BlobHandle handle;
        handle.base = context_->GetSharedWorkSpace(
            DimsVectorUtils::Count(desc.dims) * DataTypeUtils::GetBytesSize(desc.data_type), space_id++);
        nchw_blob_out.push_back(std::make_shared<Blob>(desc, handle));
    }

    return RPD_OK; 
}

Status ArmNchwLayerAcc::DoForward(const std::vector<Blob *> &inputs, const std::vector<Blob *> &outputs) {
    return Status(RPDERR_LAYER_ERR, "CALL ERROR: NCHW BASE TYPE, NOT IMPLEMENT");
}

std::vector<Blob *> ArmNchwLayerAcc::GetNchwBlobVector(const std::vector<std::shared_ptr<Blob>> &blobs) {
    std::vector<Blob *> ret;
    for (auto v : blobs) {
        ret.push_back(v.get());
    }
    return ret;
}

}  // namespace TNN_NS
