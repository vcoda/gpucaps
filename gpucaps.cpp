#include <iomanip>
#include <iostream>
#include <climits>
#include "third-party/magma/magma.h"
#include "gpucaps.h"

using std::cout;
using std::cin;
using std::endl;
using std::setw;
using std::left;

void printDeviceGroups(magma::InstancePtr instance,
    std::streamsize width)
{
    const auto& physicalDeviceGroups = instance->enumeratePhysicalDeviceGroups();
    uint32_t physicalGroupIndex = 0;
    for (const auto& physicalDeviceGroup : physicalDeviceGroups)
    {
        cout << endl;
        cout << "#" << physicalGroupIndex++ << endl << endl;
        cout << setw(width) << left << "Physical device count " << physicalDeviceGroup.physicalDeviceCount << endl;;
        cout << setw(width) << left << "Subset allocation" << booleanString(physicalDeviceGroup.subsetAllocation) << endl;;
        cout << endl;
    }
}

void printDeviceProperties(magma::PhysicalDevicePtr physicalDevice,
    uint32_t deviceId,
    std::streamsize width)
{
    const auto& properties = physicalDevice->getProperties();
    cout << endl << "=============== " << properties.deviceName << " (" << deviceId << ") ===============" << endl;
    cout << endl;
    cout << std::hex;
    cout << setw(width) << left << "API version" << versionString(properties.apiVersion) << endl;
    cout << setw(width) << left << "Driver version" << versionString(properties.driverVersion) << endl;
    cout << setw(width) << left << "Vendor ID" << vendorIDString(properties.vendorID) << " (0x" << properties.vendorID << ")" << endl;
    cout << setw(width) << left << "Device ID" << "0x" << properties.deviceID << endl;
    cout << setw(width) << left << "Device type" << magma::helpers::stringize(properties.deviceType) << endl;
    cout << std::dec;
}

void printDeviceFeatures(magma::PhysicalDevicePtr physicalDevice,
    std::streamsize width)
{
    const auto& features = physicalDevice->getFeatures();
    cout << endl;
    cout << setw(width) << left << "Robust buffer access" << booleanString(features.robustBufferAccess) << endl;
    cout << setw(width) << left << "Full draw index Uint32" << booleanString(features.fullDrawIndexUint32) << endl;
    cout << setw(width) << left << "Image cube array" << booleanString(features.imageCubeArray) << endl;
    cout << setw(width) << left << "Independent blend" << booleanString(features.independentBlend) << endl;
    cout << setw(width) << left << "Geometry shader" << booleanString(features.geometryShader) << endl;
    cout << setw(width) << left << "Tessellation shader" << booleanString(features.tessellationShader) << endl;
    cout << setw(width) << left << "Sample rate shading" << booleanString(features.sampleRateShading) << endl;
    cout << setw(width) << left << "Dual src blend" << booleanString(features.dualSrcBlend) << endl;
    cout << setw(width) << left << "Logic op" << booleanString(features.logicOp) << endl;
    cout << setw(width) << left << "Multi draw indirect" << booleanString(features.multiDrawIndirect) << endl;
    cout << setw(width) << left << "Draw indirect first instance" << booleanString(features.drawIndirectFirstInstance) << endl;
    cout << setw(width) << left << "Depth clamp" << booleanString(features.depthClamp) << endl;
    cout << setw(width) << left << "Depth bias clamp" << booleanString(features.depthBiasClamp) << endl;
    cout << setw(width) << left << "Fill mode non-solid" << booleanString(features.fillModeNonSolid) << endl;
    cout << setw(width) << left << "Depth bounds" << booleanString(features.depthBounds) << endl;
    cout << setw(width) << left << "Wide lines" << booleanString(features.wideLines) << endl;
    cout << setw(width) << left << "Large points" << booleanString(features.largePoints) << endl;
    cout << setw(width) << left << "Alpha to one" << booleanString(features.alphaToOne) << endl;
    cout << setw(width) << left << "Multi viewport" << booleanString(features.multiViewport) << endl;
    cout << setw(width) << left << "Sampler anisotropy" << booleanString(features.samplerAnisotropy) << endl;
    cout << endl;
    cout << setw(width) << left << "Texture compression ETC2" << booleanString(features.textureCompressionETC2) << endl;
    cout << setw(width) << left << "Texture compression ASTC/LDR" << booleanString(features.textureCompressionASTC_LDR) << endl;
    cout << setw(width) << left << "Texture compression BC (DXT)" << booleanString(features.textureCompressionBC) << endl;
    cout << endl;
    cout << setw(width) << left << "Occlusion query precise" << booleanString(features.occlusionQueryPrecise) << endl;
    cout << setw(width) << left << "Pipeline statistics query" << booleanString(features.pipelineStatisticsQuery) << endl;
    cout << setw(width) << left << "Vertex pipeline stores and atomics" << booleanString(features.vertexPipelineStoresAndAtomics) << endl;
    cout << setw(width) << left << "Fragment stores and atomics" << booleanString(features.fragmentStoresAndAtomics) << endl;
    cout << endl;
    cout << setw(width) << left << "Shader tessellation and geometry point size" << booleanString(features.shaderTessellationAndGeometryPointSize) << endl;
    cout << setw(width) << left << "Shader image gather extended" << booleanString(features.shaderImageGatherExtended) << endl;
    cout << setw(width) << left << "Shader storage image extended formats" << booleanString(features.shaderStorageImageExtendedFormats) << endl;
    cout << setw(width) << left << "Shader storage image multisample" << booleanString(features.shaderStorageImageMultisample) << endl;
    cout << setw(width) << left << "Shader storage image read without format" << booleanString(features.shaderStorageImageReadWithoutFormat) << endl;
    cout << setw(width) << left << "Shader storage image write without format" << booleanString(features.shaderStorageImageWriteWithoutFormat) << endl;
    cout << setw(width) << left << "Shader uniform buffer array dynamic indexing" << booleanString(features.shaderUniformBufferArrayDynamicIndexing) << endl;
    cout << setw(width) << left << "Shader sampled image array dynamic indexing" << booleanString(features.shaderSampledImageArrayDynamicIndexing) << endl;
    cout << setw(width) << left << "Shader storage buffer array dynamic indexing" << booleanString(features.shaderStorageBufferArrayDynamicIndexing) << endl;
    cout << setw(width) << left << "Shader storage image array dynamic indexing" << booleanString(features.shaderStorageImageArrayDynamicIndexing) << endl;
    cout << setw(width) << left << "Shader clip distance" << booleanString(features.shaderClipDistance) << endl;
    cout << setw(width) << left << "Shader cull distance" << booleanString(features.shaderCullDistance) << endl;
    cout << setw(width) << left << "Shader float64" << booleanString(features.shaderFloat64) << endl;
    cout << setw(width) << left << "Shader int64" << booleanString(features.shaderInt64) << endl;
    cout << setw(width) << left << "Shader int16" << booleanString(features.shaderInt16) << endl;
    cout << setw(width) << left << "Shader resource residency" << booleanString(features.shaderResourceResidency) << endl;
    cout << setw(width) << left << "Shader resource min LOD" << booleanString(features.shaderResourceMinLod) << endl;
    cout << endl;
    cout << setw(width) << left << "Sparse binding" << booleanString(features.sparseBinding) << endl;
    cout << setw(width) << left << "Sparse residency buffer" << booleanString(features.sparseResidencyBuffer) << endl;
    cout << setw(width) << left << "Sparse residency image2D" << booleanString(features.sparseResidencyImage2D) << endl;
    cout << setw(width) << left << "Sparse residency image3D" << booleanString(features.sparseResidencyImage3D) << endl;
    cout << setw(width) << left << "Sparse residency 2 samples" << booleanString(features.sparseResidency2Samples) << endl;
    cout << setw(width) << left << "Sparse residency 4 samples" << booleanString(features.sparseResidency4Samples) << endl;
    cout << setw(width) << left << "Sparse residency 8 samples" << booleanString(features.sparseResidency8Samples) << endl;
    cout << setw(width) << left << "Sparse residency 16 samples" << booleanString(features.sparseResidency16Samples) << endl;
    cout << setw(width) << left << "Sparse residency aliased" << booleanString(features.sparseResidencyAliased) << endl;
    cout << endl;
    cout << setw(width) << left << "Variable multisample rate" << booleanString(features.variableMultisampleRate) << endl;
    cout << setw(width) << left << "Inherited queries" << booleanString(features.inheritedQueries) << endl;
}

void printDeviceLimits(magma::PhysicalDevicePtr physicalDevice,
    std::streamsize width)
{
    const auto& limits = physicalDevice->getProperties().limits;
    cout << endl;
    cout << setw(width) << left << "Max image dimension 1D" << limits.maxImageDimension1D << endl;
    cout << setw(width) << left << "Max image dimension 2D" << limits.maxImageDimension2D << endl;
    cout << setw(width) << left << "Max image dimension 3D" << limits.maxImageDimension3D << endl;
    cout << setw(width) << left << "Max image dimension Cube" << limits.maxImageDimensionCube << endl;
    cout << setw(width) << left << "Max image array layers" << limits.maxImageArrayLayers << endl;
    cout << endl;
    cout << setw(width) << left << "Max texel buffer elements" << uint32String(limits.maxTexelBufferElements) << endl;
    cout << setw(width) << left << "Max uniform buffer range" << uint32String(limits.maxUniformBufferRange) << endl;
    cout << setw(width) << left << "Max storage buffer range" << uint32String(limits.maxStorageBufferRange) << endl;
    cout << endl;
    cout << setw(width) << left << "Max push constants size" << limits.maxPushConstantsSize << endl;
    cout << setw(width) << left << "Max memory allocation count" << limits.maxMemoryAllocationCount << endl;
    cout << setw(width) << left << "Max sampler allocation count" << limits.maxSamplerAllocationCount << endl;
    cout << setw(width) << left << "Buffer image granularity" << limits.bufferImageGranularity << endl;
    cout << setw(width) << left << "Sparse address space size" << limits.sparseAddressSpaceSize << endl;
    cout << setw(width) << left << "Max bound descriptor sets" << limits.maxBoundDescriptorSets << endl;
    cout << endl;
    cout << setw(width) << left << "Max per stage descriptor samplers" << uint32String(limits.maxPerStageDescriptorSamplers) << endl;
    cout << setw(width) << left << "Max per stage descriptor uniform buffers" << uint32String(limits.maxPerStageDescriptorUniformBuffers) << endl;
    cout << setw(width) << left << "Max per stage descriptor storage buffers" << uint32String(limits.maxPerStageDescriptorStorageBuffers) << endl;
    cout << setw(width) << left << "Max per stage descriptor sampled images" << uint32String(limits.maxPerStageDescriptorSampledImages) << endl;
    cout << setw(width) << left << "Max per stage descriptor storage images" << uint32String(limits.maxPerStageDescriptorStorageImages) << endl;
    cout << setw(width) << left << "Max per stage descriptor input attachments" << uint32String(limits.maxPerStageDescriptorInputAttachments) << endl;
    cout << setw(width) << left << "Max per stage resources" << uint32String(limits.maxPerStageResources) << endl;
    cout << endl;
    cout << setw(width) << left << "Max descriptor set samplers" << uint32String(limits.maxDescriptorSetSamplers) << endl;
    cout << setw(width) << left << "Max descriptor set uniform buffers" << uint32String(limits.maxDescriptorSetUniformBuffers) << endl;
    cout << setw(width) << left << "Max descriptor set uniform buffers dynamic" << uint32String(limits.maxDescriptorSetUniformBuffersDynamic) << endl;
    cout << setw(width) << left << "Max descriptor set storage buffers" << uint32String(limits.maxDescriptorSetStorageBuffers) << endl;
    cout << setw(width) << left << "Max descriptor set storage buffers dynamic" << uint32String(limits.maxDescriptorSetStorageBuffersDynamic) << endl;
    cout << setw(width) << left << "Max descriptor set sampled images" << uint32String(limits.maxDescriptorSetSampledImages) << endl;
    cout << setw(width) << left << "Max descriptor set storage images" << uint32String(limits.maxDescriptorSetStorageImages) << endl;
    cout << setw(width) << left << "Max descriptor set input attachments" << uint32String(limits.maxDescriptorSetInputAttachments) << endl;
    cout << endl;
    cout << setw(width) << left << "Max vertex input attributes" << uint32String(limits.maxVertexInputAttributes) << endl;
    cout << setw(width) << left << "Max vertex input bindings" << uint32String(limits.maxVertexInputBindings) << endl;
    cout << setw(width) << left << "Max vertex input attribute offset" << uint32String(limits.maxVertexInputAttributeOffset) << endl;
    cout << setw(width) << left << "Max vertex input binding stride" << limits.maxVertexInputBindingStride << endl;
    cout << setw(width) << left << "Max vertex output components" << limits.maxVertexOutputComponents << endl;
    cout << endl;
    cout << setw(width + 10) << left << "Max tessellation generation level" << limits.maxTessellationGenerationLevel << endl;
    cout << setw(width + 10) << left << "Max tessellation patchSize" << limits.maxTessellationPatchSize << endl;
    cout << setw(width + 10) << left << "Max tessellation control per vertex input components" << limits.maxTessellationControlPerVertexInputComponents << endl;
    cout << setw(width + 10) << left << "Max tessellation control per vertex output components" << limits.maxTessellationControlPerVertexOutputComponents << endl;
    cout << setw(width + 10) << left << "Max tessellation control per patch output components" << limits.maxTessellationControlPerPatchOutputComponents << endl;
    cout << setw(width + 10) << left << "Max tessellation control total output components" << limits.maxTessellationControlTotalOutputComponents << endl;
    cout << setw(width + 10) << left << "Max tessellation evaluation input components" << limits.maxTessellationEvaluationInputComponents << endl;
    cout << setw(width + 10) << left << "Max tessellation evaluation output components" << limits.maxTessellationEvaluationOutputComponents << endl;
    cout << endl;
    cout << setw(width) << left << "Max geometry shader invocations" << limits.maxGeometryShaderInvocations << endl;
    cout << setw(width) << left << "Max geometry input components" << limits.maxGeometryInputComponents << endl;
    cout << setw(width) << left << "Max geometry output components" << limits.maxGeometryOutputComponents << endl;
    cout << setw(width) << left << "Max geometry output vertices" << limits.maxGeometryOutputVertices << endl;
    cout << setw(width) << left << "Max geometry total output components" << limits.maxGeometryTotalOutputComponents << endl;
    cout << endl;
    cout << setw(width) << left << "Max fragment input components" << limits.maxFragmentInputComponents << endl;
    cout << setw(width) << left << "Max fragment output attachments" << limits.maxFragmentOutputAttachments << endl;
    cout << setw(width) << left << "Max fragment dual src attachments" << limits.maxFragmentDualSrcAttachments << endl;
    cout << setw(width) << left << "Max fragment combined output resources" << uint32String(limits.maxFragmentCombinedOutputResources) << endl;
    cout << endl;
    cout << setw(width) << left << "Max compute shared memory size" << limits.maxComputeSharedMemorySize << endl;
    cout << setw(width) << left << "Max compute workgroup count" << "[" <<
        limits.maxComputeWorkGroupCount[0] << ", " <<
        limits.maxComputeWorkGroupCount[1] << ", " <<
        limits.maxComputeWorkGroupCount[2] << "]" << endl;
    cout << setw(width) << left << "Max compute workgroup invocations" << limits.maxComputeWorkGroupInvocations << endl;
    cout << setw(width) << left << "Max compute workgroup size" << "[" <<
        limits.maxComputeWorkGroupSize[0] << ", " <<
        limits.maxComputeWorkGroupSize[1] << ", " <<
        limits.maxComputeWorkGroupSize[2] << "]" << endl;
    cout << endl;
    cout << setw(width) << left << "Sub-pixel precision bits" << limits.subPixelPrecisionBits << endl;
    cout << setw(width) << left << "Sub-texel precision bits " << limits.subTexelPrecisionBits << endl;
    cout << setw(width) << left << "Mipmap precision bits" << limits.mipmapPrecisionBits << endl;
    cout << endl;
    cout << setw(width) << left << "Max draw indexed index value" << uint32String(limits.maxDrawIndexedIndexValue) << endl;
    cout << setw(width) << left << "Max draw indirect count" << uint32String(limits.maxDrawIndirectCount) << endl;
    cout << endl;
    cout << setw(width) << left << "Max sampler lod bias" << limits.maxSamplerLodBias << endl;
    cout << setw(width) << left << "Max sampler anisotropy" << limits.maxSamplerAnisotropy << endl;
    cout << endl;
    cout << setw(width) << left << "Max viewports" << limits.maxViewports << endl;
    cout << setw(width) << left << "Max viewport dimensions" << "[" <<
        limits.maxViewportDimensions[0] << ", " <<
        limits.maxViewportDimensions[1] << "]" << endl;
    cout << setw(width) << left << "Viewport bounds range" << "[" <<
        limits.viewportBoundsRange[0] << ", " <<
        limits.viewportBoundsRange[1] << "]" << endl;
    cout << setw(width) << left << "Viewport sub-pixel bits" << limits.viewportSubPixelBits << endl;
    cout << endl;
    cout << setw(width) << left << "Min memory map alignment" << limits.minMemoryMapAlignment << endl;
    cout << setw(width) << left << "Min texel buffer offset alignment" << limits.minTexelBufferOffsetAlignment << endl;
    cout << setw(width) << left << "Min uniform buffer offset alignment" << limits.minUniformBufferOffsetAlignment << endl;
    cout << setw(width) << left << "Min storage buffer offset alignment" << limits.minStorageBufferOffsetAlignment << endl;
    cout << endl;
    cout << setw(width) << left << "Min texel offset" << limits.minTexelOffset << endl;
    cout << setw(width) << left << "Max texel offset" << limits.maxTexelOffset << endl;
    cout << setw(width) << left << "Min texel gather offset" << limits.minTexelGatherOffset << endl;
    cout << setw(width) << left << "Max texel gather offset" << limits.maxTexelGatherOffset << endl;
    cout << setw(width) << left << "Min interpolation offset" << limits.minInterpolationOffset << endl;
    cout << setw(width) << left << "Max interpolation offset" << limits.maxInterpolationOffset << endl;
    cout << setw(width) << left << "Sub-pixel interpolation offset bits" << limits.subPixelInterpolationOffsetBits << endl;
    cout << endl;
    cout << setw(width) << left << "Max framebuffer width" << limits.maxFramebufferWidth << endl;
    cout << setw(width) << left << "Max framebuffer height" << limits.maxFramebufferHeight << endl;
    cout << setw(width) << left << "Max framebuffer layers" << limits.maxFramebufferLayers << endl;
    cout << setw(width) << left << "Framebuffer color sample counts" << limits.framebufferColorSampleCounts << endl;
    cout << setw(width) << left << "Framebuffer depth sample counts" << limits.framebufferDepthSampleCounts << endl;
    cout << setw(width) << left << "Framebuffer stencil sample counts" << limits.framebufferStencilSampleCounts << endl;
    cout << setw(width) << left << "Framebuffer no attachments sample counts" << limits.framebufferNoAttachmentsSampleCounts << endl;
    cout << setw(width) << left << "Max color attachments" << limits.maxColorAttachments << endl;
    cout << endl;
    cout << setw(width) << left << "Sampled image color sample counts" << limits.sampledImageColorSampleCounts << endl;
    cout << setw(width) << left << "Sampled image integer sample counts" << limits.sampledImageIntegerSampleCounts << endl;
    cout << setw(width) << left << "Sampled image depth sample counts" << limits.sampledImageDepthSampleCounts << endl;
    cout << setw(width) << left << "Sampled image stencil sample counts" << limits.sampledImageStencilSampleCounts << endl;
    cout << setw(width) << left << "Storage image sample counts" << limits.storageImageSampleCounts << endl;
    cout << setw(width) << left << "Max sample mask words" << limits.maxSampleMaskWords << endl;
    cout << endl;
    cout << setw(width) << left << "Timestamp compute and graphics" << booleanString(limits.timestampComputeAndGraphics) << endl;
    cout << setw(width) << left << "Timestamp period" << limits.timestampPeriod << endl;
    cout << endl;
    cout << setw(width) << left << "Max clip distances" << limits.maxClipDistances << endl;
    cout << setw(width) << left << "Max cull distances" << limits.maxCullDistances << endl;
    cout << setw(width) << left << "Max combined clip and cull distances" << limits.maxCombinedClipAndCullDistances << endl;
    cout << endl;
    cout << setw(width) << left << "Discrete queue priorities" << limits.discreteQueuePriorities << endl;
    cout << endl;
    cout << setw(width) << left << "Point size range" << "[" <<
        limits.pointSizeRange[0] << ", " <<
        limits.pointSizeRange[1] << "]" << endl;
    cout << setw(width) << left << "Line width range" << "[" <<
        limits.lineWidthRange[0] << ", " <<
        limits.lineWidthRange[1] << "]" << endl;
    cout << setw(width) << left << "Point size granularity" << limits.pointSizeGranularity << endl;
    cout << setw(width) << left << "Line width granularity" << limits.lineWidthGranularity << endl;
    cout << setw(width) << left << "Strict lines" << booleanString(limits.strictLines) << endl;
    cout << endl;
    cout << setw(width) << left << "Standard sample locations" << booleanString(limits.standardSampleLocations) << endl;
    cout << setw(width) << left << "Optimal buffer copy offset alignment" << limits.optimalBufferCopyOffsetAlignment << endl;
    cout << setw(width) << left << "Optimal buffer copy row pitch alignment" << limits.optimalBufferCopyRowPitchAlignment << endl;
    cout << setw(width) << left << "Non-coherent atom size" << limits.nonCoherentAtomSize << endl;
}

void printQueueFamilyProperties(magma::PhysicalDevicePtr physicalDevice,
    std::streamsize width)
{
    const auto& queueFamilyProperties = physicalDevice->getQueueFamilyProperties();
    uint32_t queueFamilyIndex = 0;
    for (const auto& properties : queueFamilyProperties)
    {
        cout << endl << "#" << queueFamilyIndex << endl << endl;
        cout << "Queue flags";
        for (const auto bit : {
            VK_QUEUE_GRAPHICS_BIT,
            VK_QUEUE_COMPUTE_BIT,
            VK_QUEUE_TRANSFER_BIT,
            VK_QUEUE_SPARSE_BINDING_BIT,
            VK_QUEUE_PROTECTED_BIT})
        {
            if (properties.queueFlags & bit)
            {
                cout << endl;
                cout << '\t' << magma::helpers::stringize(bit);
            }
        }
        cout << endl;
        cout << setw(width) << left << "Queue count" << properties.queueCount << endl;
        cout << setw(width) << left << "Timestamp valid bits" << properties.timestampValidBits << endl;
        cout << setw(width) << left << "Min image transfer granularity" << "[" <<
            properties.minImageTransferGranularity.width << ", " <<
            properties.minImageTransferGranularity.height << ", " <<
            properties.minImageTransferGranularity.depth << "]" << endl;
#ifdef VK_USE_PLATFORM_WIN32_KHR
        // On Win32 doesn't requires display and visual ID
        const bool supportsPresentation = physicalDevice->getPresentationSupport(queueFamilyIndex, nullptr);
        cout << setw(width) << left << "Supports presentation" << booleanString(supportsPresentation) << endl;
#endif
        ++queueFamilyIndex;
    }
}

void printDeviceMemoryTypes(magma::PhysicalDevicePtr physicalDevice)
{
    const auto& properties = physicalDevice->getMemoryProperties();
    for (uint32_t i = 0; i < properties.memoryTypeCount; ++i)
    {
        const VkMemoryType& memoryType = properties.memoryTypes[i];
        cout << endl;
        cout << "#" << i << endl << endl;
        cout << "Properties";
        bool hasFlags = false;
        for (const auto bit : {
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            VK_MEMORY_PROPERTY_HOST_CACHED_BIT,
            VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT,
            VK_MEMORY_PROPERTY_PROTECTED_BIT})
        {
            if (memoryType.propertyFlags & bit)
            {
                cout << endl;
                cout << '\t' << magma::helpers::stringize(bit);
                hasFlags = true;
            }
        }
        if (!hasFlags)
        {
            cout << endl;
            cout << '\t' << "---";
        }
        cout << endl;
        cout << "Heap index " << memoryType.heapIndex << endl;
    }
}

void printDeviceMemoryHeaps(magma::PhysicalDevicePtr physicalDevice)
{
    const auto& properties = physicalDevice->getMemoryProperties();
    for (uint32_t i = 0; i < properties.memoryHeapCount; ++i)
    {
        const VkMemoryHeap& memoryHeap = properties.memoryHeaps[i];
        cout << endl;
        cout << "#" << i << endl << endl;
        cout << "Heap size " << memoryHeap.size << endl;
        cout << "Heap flags";
        bool hasFlags = false;
        for (const auto bit : {
            VK_MEMORY_HEAP_DEVICE_LOCAL_BIT,
            VK_MEMORY_HEAP_MULTI_INSTANCE_BIT,
            VK_MEMORY_HEAP_MULTI_INSTANCE_BIT_KHR})
        {
            if (memoryHeap.flags & bit)
            {
                cout << endl;
                cout << '\t' << magma::helpers::stringize(bit);
                hasFlags = true;
            }
        }
        if (!hasFlags)
        {
            cout << endl;
            cout << '\t' << "---";
        }
        cout << endl;
    }
}

void printShaderCoreProperties(magma::PhysicalDevicePtr physicalDevice,
    std::streamsize width)
{
    const auto& properties = physicalDevice->getShaderCoreProperties();
    cout << endl;
    cout << setw(width) << left << "Shader engine count" << properties.shaderEngineCount << endl;
    cout << setw(width) << left << "Shader arrays per engine count" << properties.shaderArraysPerEngineCount << endl;
    cout << setw(width) << left << "Compute units per shader array" << properties.computeUnitsPerShaderArray << endl;
    cout << setw(width) << left << "SIMD per compute unit" << properties.simdPerComputeUnit << endl;
    cout << endl;
    cout << setw(width) << left << "Wavefronts per SIMD" << properties.wavefrontsPerSimd << endl;
    cout << setw(width) << left << "Wavefront size" << properties.wavefrontSize << endl;
    cout << endl;
    cout << setw(width) << left << "SGPRs per SIMD" << properties.sgprsPerSimd << endl;
    cout << setw(width) << left << "Min SGPR allocations" << properties.minSgprAllocation << endl;
    cout << setw(width) << left << "Max SGPR allocations" << properties.maxSgprAllocation << endl;
    cout << setw(width) << left << "SGPR allocation granularity" << properties.sgprAllocationGranularity << endl;
    cout << endl;
    cout << setw(width) << left << "VGPRs per SIMD" << properties.vgprsPerSimd << endl;
    cout << setw(width) << left << "Min VGPR allocation" << properties.minVgprAllocation << endl;
    cout << setw(width) << left << "Max VGPR allocation" << properties.maxVgprAllocation << endl;
    cout << setw(width) << left << "VGPR allocation granularity" << properties.vgprAllocationGranularity << endl;
}

void printExtendedShaderCoreProperties(magma::PhysicalDevicePtr physicalDevice,
    std::streamsize width)
{
    physicalDevice;
    width;
#ifdef VK_AMD_shader_core_properties2
    const auto& properties = physicalDevice->getShaderCoreProperties2();
    cout << endl;
    cout << setw(width) << left << "Active compute unit count" << properties.activeComputeUnitCount << endl;
#endif
}

void printExtensions(std::shared_ptr<magma::Extensions> extensions,
    std::streamsize width)
{
    cout << endl;
    extensions->forEach(
        [width](const std::string& extensionName, uint32_t specVersion)
        {
            cout << setw(width) << left << extensionName << "spec version "
                << specVersion << endl;
        });
}

magma::InstancePtr createInstance()
{
    const std::vector<const char *> layerNames = {
#ifdef _DEBUG
        "VK_LAYER_LUNARG_standard_validation"
#endif
    };
    auto instanceExtensions = std::make_unique<magma::InstanceExtensions>();
    std::vector<const char *> extensions = {
#ifdef VK_USE_PLATFORM_WIN32_KHR
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#endif
    };
    if (instanceExtensions->KHR_get_physical_device_properties2)
        extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    if (instanceExtensions->KHR_device_group_creation)
        extensions.push_back(VK_KHR_DEVICE_GROUP_CREATION_EXTENSION_NAME);
    return std::make_shared<magma::Instance>("gpucaps", "magma", VK_API_VERSION_1_0, layerNames, extensions);
}

int main()
{
    cout << "Vulkan GPU Caps Viewer [Version 1.0]" << endl;
    cout << "(c) 2018-2020 Victor Coda." << endl;

    auto instance = createInstance();
    if (!instance)
        return -1;
    auto instanceExtensions = std::make_shared<magma::InstanceExtensions>();
    cout << endl << "==================== Instance Extensions ====================" << endl;
    printExtensions(instanceExtensions, 45);
    if (instanceExtensions->KHR_device_group_creation)
    {
        cout << endl << "==================== Device Groups ====================" << endl;
        printDeviceGroups(instance, 45);
    }
    const uint32_t physicalDeviceCount = instance->countPhysicalDevices();
    for (uint32_t deviceId = 0; deviceId < physicalDeviceCount; ++deviceId)
    {
        magma::PhysicalDevicePtr physicalDevice = instance->getPhysicalDevice(deviceId);
        printDeviceProperties(physicalDevice, deviceId, 15);
        cout << endl << "==================== Device Features ====================" << endl;
        printDeviceFeatures(physicalDevice, 45);
        cout << endl << "==================== Device Limits ====================" << endl;
        printDeviceLimits(physicalDevice, 45);
        cout << endl << "==================== Queue Family Properties ====================" << endl;
        printQueueFamilyProperties(physicalDevice, 45);
        cout << endl << "==================== Device Memory Types ====================" << endl;
        printDeviceMemoryTypes(physicalDevice);
        cout << endl << "==================== Device Memory Heaps ====================" << endl;
        printDeviceMemoryHeaps(physicalDevice);
        auto deviceExtensions = std::make_shared<magma::PhysicalDeviceExtensions>(physicalDevice);
        if (deviceExtensions->AMD_shader_core_properties)
        {
            cout << endl << "==================== Shader Core Properties ====================" << endl;
            printShaderCoreProperties(physicalDevice, 35);
            if (deviceExtensions->AMD_shader_core_properties2)
                printExtendedShaderCoreProperties(physicalDevice, 35);
        }
        cout << endl << "==================== Device Extensions ====================" << endl;
        printExtensions(deviceExtensions, 45);
        if (physicalDeviceCount > 1 && deviceId < physicalDeviceCount - 1)
        {
            cout << "Print any key for the next device";
            cin.get();
        }
    }
    return 0;
}
