#include "gpucaps.h"
#include "third-party/magma/magma.h"

// https://www.reddit.com/r/vulkan/comments/4ta9nj/is_there_a_comprehensive_list_of_the_names_and/
enum Vendor : uint16_t
{
    AMD = 0x1002,
    ImaginationTechnologies = 0x1010,
    NVidia = 0x10DE,
    ARM = 0x13B5,
    Qualcomm = 0x5143,
    Intel = 0x8086
};

std::string apiVersionString(uint32_t apiVersion)
{
    const uint32_t major = VK_VERSION_MAJOR(apiVersion);
    const uint32_t minor = VK_VERSION_MINOR(apiVersion);
    const uint32_t patch = VK_VERSION_PATCH(apiVersion);
    return std::to_string(major) + "." +
           std::to_string(minor) + "." +
           std::to_string(patch);
}

// The encoding of driverVersion is implementation-defined. It may not use the same encoding as apiVersion.
// Applications should follow information from the vendor on how to extract the version information from driverVersion.
std::string driverVersionString(uint32_t driverVersion, uint32_t vendorID)
{
    // https://www.reddit.com/r/vulkan/comments/fmift4/how_to_decode_driverversion_field_of/
    const uint16_t pciVendorID = vendorID & 0xFFFF;
    if (Vendor::NVidia == pciVendorID)
    {   // NVIDIA
        const uint32_t major = (driverVersion >> 22) & 0b1111111111; // 10
        const uint32_t minor = (driverVersion >> 14) & 0b11111111; // 8
        const uint32_t patch = (driverVersion >> 6)  & 0b11111111; // 8
        const uint32_t build = (driverVersion)       & 0b111111; // 6
        return std::to_string(major) + "." +
               std::to_string(minor) + "." +
               std::to_string(patch) + "." +
               std::to_string(build);
    } else if (Vendor::Intel == pciVendorID)
    {   // Intel
        const uint32_t major = driverVersion >> 14;
        const uint32_t minor = driverVersion & 0b11111111111111; // 14
        return std::to_string(major) + "." +
               std::to_string(minor);
    } else
    {   // AMD & others
        const uint32_t major = (driverVersion >> 22) & 0b1111111111; // 10
        const uint32_t minor = (driverVersion >> 12) & 0b1111111111; // 10
        const uint32_t patch = (driverVersion)       & 0b111111111111; // 12
        return std::to_string(major) + "." +
               std::to_string(minor) + "." +
               std::to_string(patch);
    }
}

std::string vendorName(uint32_t vendorID)
{
    const uint16_t pciVendorID = vendorID & 0xFFFF;
    switch (pciVendorID)
    {
    case Vendor::AMD: return "AMD";
    case Vendor::ImaginationTechnologies: return "ImgTec";
    case Vendor::NVidia: return "NVidia";
    case Vendor::ARM: return "ARM";
    case Vendor::Qualcomm: return "Qualcomm";
    case Vendor::Intel: return "Intel";
    }
    return "Unknown";
}

void printDeviceGroups(magma::InstancePtr instance)
{
    const auto physicalDeviceGroups = instance->enumeratePhysicalDeviceGroups();
    uint32_t physicalGroupIndex = 0;
    for (const auto& physicalDeviceGroup : physicalDeviceGroups)
    {
        printEndLn();
        std::cout << "#" << physicalGroupIndex++ << std::endl << std::endl;
        printLn("Physical device count ", physicalDeviceGroup.physicalDeviceCount);
        printLn("Subset allocation", booleanString(physicalDeviceGroup.subsetAllocation));
        printEndLn();
    }
}

void printDeviceProperties(magma::PhysicalDevicePtr physicalDevice, uint32_t deviceId)
{
    const auto properties = physicalDevice->getProperties();
    printHeading((std::string(properties.deviceName) + " (" + std::to_string(deviceId) + ")").c_str());
    printEndLn();
    printLn("API version", apiVersionString(properties.apiVersion));
    printLn("Driver version", driverVersionString(properties.driverVersion, properties.vendorID));
    std::cout << std::hex;
    std::cout << std::setw(width) << std::left << "Vendor ID" << "0x" << properties.vendorID << " (" << vendorName(properties.vendorID) << ")" << std::endl;
    std::cout << std::setw(width) << std::left << "Device ID" << "0x" << properties.deviceID << std::endl;
    std::cout << std::dec;
    printLn("Device type", magma::helpers::stringize(properties.deviceType));
}

void printDriverProperties(magma::PhysicalDevicePtr physicalDevice)
{
#ifdef VK_KHR_driver_properties
    const auto properties = physicalDevice->getDriverProperties();
    printEndLn();
    printLn("Driver ID", properties.driverID);
    printLn("Driver name", properties.driverName);
    printLn("Driver info", properties.driverInfo);
    std::cout << std::setw(width) << std::left << "Conformance version"
        << (uint32_t)properties.conformanceVersion.major << "."
        << (uint32_t)properties.conformanceVersion.minor << "."
        << (uint32_t)properties.conformanceVersion.subminor << "."
        << (uint32_t)properties.conformanceVersion.patch << std::endl;
#endif // VK_KHR_driver_properties
}

void printDeviceFeatures(magma::PhysicalDevicePtr physicalDevice)
{
    const auto features = physicalDevice->getFeatures();
    printEndLn();
    printLn("Robust buffer access", booleanString(features.robustBufferAccess));
    printLn("Full draw index uint32", booleanString(features.fullDrawIndexUint32));
    printLn("Image cube array", booleanString(features.imageCubeArray));
    printLn("Independent blend", booleanString(features.independentBlend));
    printLn("Geometry shader", booleanString(features.geometryShader));
    printLn("Tessellation shader", booleanString(features.tessellationShader));
    printLn("Sample rate shading", booleanString(features.sampleRateShading));
    printLn("Dual src blend", booleanString(features.dualSrcBlend));
    printLn("Logic op", booleanString(features.logicOp));
    printLn("Multi draw indirect", booleanString(features.multiDrawIndirect));
    printLn("Draw indirect first instance", booleanString(features.drawIndirectFirstInstance));
    printLn("Depth clamp", booleanString(features.depthClamp));
    printLn("Depth bias clamp", booleanString(features.depthBiasClamp));
    printLn("Fill mode non-solid", booleanString(features.fillModeNonSolid));
    printLn("Depth bounds", booleanString(features.depthBounds));
    printLn("Wide lines", booleanString(features.wideLines));
    printLn("Large points", booleanString(features.largePoints));
    printLn("Alpha to one", booleanString(features.alphaToOne));
    printLn("Multi viewport", booleanString(features.multiViewport));
    printLn("Sampler anisotropy", booleanString(features.samplerAnisotropy));
    printEndLn();
    printLn("Texture compression ETC2", booleanString(features.textureCompressionETC2));
    printLn("Texture compression ASTC/LDR", booleanString(features.textureCompressionASTC_LDR));
    printLn("Texture compression BC", booleanString(features.textureCompressionBC));
    printEndLn();
    printLn("Occlusion query precise", booleanString(features.occlusionQueryPrecise));
    printLn("Pipeline statistics query", booleanString(features.pipelineStatisticsQuery));
    printLn("Vertex pipeline stores and atomics", booleanString(features.vertexPipelineStoresAndAtomics));
    printLn("Fragment stores and atomics", booleanString(features.fragmentStoresAndAtomics));
    printEndLn();
    printLn("Shader tessellation and geometry point size", booleanString(features.shaderTessellationAndGeometryPointSize));
    printLn("Shader image gather extended", booleanString(features.shaderImageGatherExtended));
    printLn("Shader storage image extended formats", booleanString(features.shaderStorageImageExtendedFormats));
    printLn("Shader storage image multisample", booleanString(features.shaderStorageImageMultisample));
    printLn("Shader storage image read without format", booleanString(features.shaderStorageImageReadWithoutFormat));
    printLn("Shader storage image write without format", booleanString(features.shaderStorageImageWriteWithoutFormat));
    printLn("Shader uniform buffer array dynamic indexing", booleanString(features.shaderUniformBufferArrayDynamicIndexing));
    printLn("Shader sampled image array dynamic indexing", booleanString(features.shaderSampledImageArrayDynamicIndexing));
    printLn("Shader storage buffer array dynamic indexing", booleanString(features.shaderStorageBufferArrayDynamicIndexing));
    printLn("Shader storage image array dynamic indexing", booleanString(features.shaderStorageImageArrayDynamicIndexing));
    printLn("Shader clip distance", booleanString(features.shaderClipDistance));
    printLn("Shader cull distance", booleanString(features.shaderCullDistance));
    printLn("Shader float64", booleanString(features.shaderFloat64));
    printLn("Shader int64", booleanString(features.shaderInt64));
    printLn("Shader int16", booleanString(features.shaderInt16));
    printLn("Shader resource residency", booleanString(features.shaderResourceResidency));
    printLn("Shader resource min LOD", booleanString(features.shaderResourceMinLod));
    printEndLn();
    printLn("Sparse binding", booleanString(features.sparseBinding));
    printLn("Sparse residency buffer", booleanString(features.sparseResidencyBuffer));
    printLn("Sparse residency image2D", booleanString(features.sparseResidencyImage2D));
    printLn("Sparse residency image3D", booleanString(features.sparseResidencyImage3D));
    printLn("Sparse residency 2 samples", booleanString(features.sparseResidency2Samples));
    printLn("Sparse residency 4 samples", booleanString(features.sparseResidency4Samples));
    printLn("Sparse residency 8 samples", booleanString(features.sparseResidency8Samples));
    printLn("Sparse residency 16 samples", booleanString(features.sparseResidency16Samples));
    printLn("Sparse residency aliased", booleanString(features.sparseResidencyAliased));
    printEndLn();
    printLn("Variable multisample rate", booleanString(features.variableMultisampleRate));
    printLn("Inherited queries", booleanString(features.inheritedQueries));
}

void printDeviceLimits(magma::PhysicalDevicePtr physicalDevice)
{
    const auto limits = physicalDevice->getProperties().limits;
    printEndLn();
    printLn("Max image dimension 1D", limits.maxImageDimension1D);
    printLn("Max image dimension 2D", limits.maxImageDimension2D);
    printLn("Max image dimension 3D", limits.maxImageDimension3D);
    printLn("Max image dimension cube", limits.maxImageDimensionCube);
    printLn("Max image array layers", limits.maxImageArrayLayers);
    printEndLn();
    printLn("Max texel buffer elements", uint32String(limits.maxTexelBufferElements));
    printLn("Max uniform buffer range", uint32String(limits.maxUniformBufferRange));
    printLn("Max storage buffer range", uint32String(limits.maxStorageBufferRange));
    printEndLn();
    printLn("Max push constants size", limits.maxPushConstantsSize);
    printLn("Max memory allocation count", limits.maxMemoryAllocationCount);
    printLn("Max sampler allocation count", limits.maxSamplerAllocationCount);
    printLn("Buffer image granularity", limits.bufferImageGranularity);
    printLn("Sparse address space size", limits.sparseAddressSpaceSize);
    printLn("Max bound descriptor sets", limits.maxBoundDescriptorSets);
    printEndLn();
    printLn("Max per stage descriptor samplers", uint32String(limits.maxPerStageDescriptorSamplers));
    printLn("Max per stage descriptor uniform buffers", uint32String(limits.maxPerStageDescriptorUniformBuffers));
    printLn("Max per stage descriptor storage buffers", uint32String(limits.maxPerStageDescriptorStorageBuffers));
    printLn("Max per stage descriptor sampled images", uint32String(limits.maxPerStageDescriptorSampledImages));
    printLn("Max per stage descriptor storage images", uint32String(limits.maxPerStageDescriptorStorageImages));
    printLn("Max per stage descriptor input attachments", uint32String(limits.maxPerStageDescriptorInputAttachments));
    printLn("Max per stage resources", uint32String(limits.maxPerStageResources));
    printEndLn();
    printLn("Max descriptor set samplers", uint32String(limits.maxDescriptorSetSamplers));
    printLn("Max descriptor set uniform buffers", uint32String(limits.maxDescriptorSetUniformBuffers));
    printLn("Max descriptor set uniform buffers dynamic", uint32String(limits.maxDescriptorSetUniformBuffersDynamic));
    printLn("Max descriptor set storage buffers", uint32String(limits.maxDescriptorSetStorageBuffers));
    printLn("Max descriptor set storage buffers dynamic", uint32String(limits.maxDescriptorSetStorageBuffersDynamic));
    printLn("Max descriptor set sampled images", uint32String(limits.maxDescriptorSetSampledImages));
    printLn("Max descriptor set storage images", uint32String(limits.maxDescriptorSetStorageImages));
    printLn("Max descriptor set input attachments", uint32String(limits.maxDescriptorSetInputAttachments));
    printEndLn();
    printLn("Max vertex input attributes", uint32String(limits.maxVertexInputAttributes));
    printLn("Max vertex input bindings", uint32String(limits.maxVertexInputBindings));
    printLn("Max vertex input attribute offset", uint32String(limits.maxVertexInputAttributeOffset));
    printLn("Max vertex input binding stride", limits.maxVertexInputBindingStride);
    printLn("Max vertex output components", limits.maxVertexOutputComponents);
    printEndLn();
    printLn("Max tessellation generation level", limits.maxTessellationGenerationLevel);
    printLn("Max tessellation patchSize", limits.maxTessellationPatchSize);
    printEndLn();
    printLn("Max tessellation control per vertex input components", limits.maxTessellationControlPerVertexInputComponents);
    printLn("Max tessellation control per vertex output components", limits.maxTessellationControlPerVertexOutputComponents);
    printLn("Max tessellation control per patch output components", limits.maxTessellationControlPerPatchOutputComponents);
    printLn("Max tessellation control total output components", limits.maxTessellationControlTotalOutputComponents);
    printLn("Max tessellation evaluation input components", limits.maxTessellationEvaluationInputComponents);
    printLn("Max tessellation evaluation output components", limits.maxTessellationEvaluationOutputComponents);
    printEndLn();
    printLn("Max geometry shader invocations", limits.maxGeometryShaderInvocations);
    printLn("Max geometry input components", limits.maxGeometryInputComponents);
    printLn("Max geometry output components", limits.maxGeometryOutputComponents);
    printLn("Max geometry output vertices", limits.maxGeometryOutputVertices);
    printLn("Max geometry total output components", limits.maxGeometryTotalOutputComponents);
    printEndLn();
    printLn("Max fragment input components", limits.maxFragmentInputComponents);
    printLn("Max fragment output attachments", limits.maxFragmentOutputAttachments);
    printLn("Max fragment dual src attachments", limits.maxFragmentDualSrcAttachments);
    printLn("Max fragment combined output resources", uint32String(limits.maxFragmentCombinedOutputResources));
    printEndLn();
    printLn("Max compute shared memory size", limits.maxComputeSharedMemorySize);
    printLn("Max compute workgroup count",
        uint32String(limits.maxComputeWorkGroupCount[0]),
        uint32String(limits.maxComputeWorkGroupCount[1]),
        uint32String(limits.maxComputeWorkGroupCount[2]));
    printLn("Max compute workgroup invocations", limits.maxComputeWorkGroupInvocations);
    printLn("Max compute workgroup size",
        limits.maxComputeWorkGroupSize[0],
        limits.maxComputeWorkGroupSize[1],
        limits.maxComputeWorkGroupSize[2]);
    printEndLn();
    printLn("Sub-pixel precision bits", limits.subPixelPrecisionBits);
    printLn("Sub-texel precision bits ", limits.subTexelPrecisionBits);
    printLn("Mipmap precision bits", limits.mipmapPrecisionBits);
    printEndLn();
    printLn("Max draw indexed index value", uint32String(limits.maxDrawIndexedIndexValue));
    printLn("Max draw indirect count", uint32String(limits.maxDrawIndirectCount));
    printEndLn();
    printLn("Max sampler lod bias", limits.maxSamplerLodBias);
    printLn("Max sampler anisotropy", limits.maxSamplerAnisotropy);
    printEndLn();
    printLn("Max viewports", limits.maxViewports);
    printLn("Max viewport dimensions",
        uint32String(limits.maxViewportDimensions[0]),
        uint32String(limits.maxViewportDimensions[1]));
    printLn("Viewport bounds range", limits.viewportBoundsRange[0], limits.viewportBoundsRange[1]);
    printLn("Viewport sub-pixel bits", limits.viewportSubPixelBits);
    printEndLn();
    printLn("Min memory map alignment", limits.minMemoryMapAlignment);
    printLn("Min texel buffer offset alignment", limits.minTexelBufferOffsetAlignment);
    printLn("Min uniform buffer offset alignment", limits.minUniformBufferOffsetAlignment);
    printLn("Min storage buffer offset alignment", limits.minStorageBufferOffsetAlignment);
    printEndLn();
    printLn("Min texel offset", limits.minTexelOffset);
    printLn("Max texel offset", limits.maxTexelOffset);
    printLn("Min texel gather offset", limits.minTexelGatherOffset);
    printLn("Max texel gather offset", limits.maxTexelGatherOffset);
    printLn("Min interpolation offset", limits.minInterpolationOffset);
    printLn("Max interpolation offset", limits.maxInterpolationOffset);
    printLn("Sub-pixel interpolation offset bits", limits.subPixelInterpolationOffsetBits);
    printEndLn();
    printLn("Max framebuffer width", limits.maxFramebufferWidth);
    printLn("Max framebuffer height", limits.maxFramebufferHeight);
    printLn("Max framebuffer layers", limits.maxFramebufferLayers);
    printLn("Framebuffer color sample counts", limits.framebufferColorSampleCounts);
    printLn("Framebuffer depth sample counts", limits.framebufferDepthSampleCounts);
    printLn("Framebuffer stencil sample counts", limits.framebufferStencilSampleCounts);
    printLn("Framebuffer no attachments sample counts", limits.framebufferNoAttachmentsSampleCounts);
    printLn("Max color attachments", limits.maxColorAttachments);
    printEndLn();
    printLn("Sampled image color sample counts", limits.sampledImageColorSampleCounts);
    printLn("Sampled image integer sample counts", limits.sampledImageIntegerSampleCounts);
    printLn("Sampled image depth sample counts", limits.sampledImageDepthSampleCounts);
    printLn("Sampled image stencil sample counts", limits.sampledImageStencilSampleCounts);
    printLn("Storage image sample counts", limits.storageImageSampleCounts);
    printLn("Max sample mask words", limits.maxSampleMaskWords);
    printEndLn();
    printLn("Timestamp compute and graphics", booleanString(limits.timestampComputeAndGraphics));
    printLn("Timestamp period", limits.timestampPeriod);
    printEndLn();
    printLn("Max clip distances", limits.maxClipDistances);
    printLn("Max cull distances", limits.maxCullDistances);
    printLn("Max combined clip and cull distances", limits.maxCombinedClipAndCullDistances);
    printEndLn();
    printLn("Discrete queue priorities", limits.discreteQueuePriorities);
    printEndLn();
    printLn("Point size range", limits.pointSizeRange[0], limits.pointSizeRange[1]);
    printLn("Line width range", limits.lineWidthRange[0], limits.lineWidthRange[1]);
    printLn("Point size granularity", limits.pointSizeGranularity);
    printLn("Line width granularity", limits.lineWidthGranularity);
    printLn("Strict lines", booleanString(limits.strictLines));
    printEndLn();
    printLn("Standard sample locations", booleanString(limits.standardSampleLocations));
    printLn("Optimal buffer copy offset alignment", limits.optimalBufferCopyOffsetAlignment);
    printLn("Optimal buffer copy row pitch alignment", limits.optimalBufferCopyRowPitchAlignment);
    printLn("Non-coherent atom size", limits.nonCoherentAtomSize);
}

void printQueueFamilyProperties(magma::PhysicalDevicePtr physicalDevice)
{
    const auto queueFamilyProperties = physicalDevice->getQueueFamilyProperties();
    uint32_t queueFamilyIndex = 0;
    for (const auto& properties : queueFamilyProperties)
    {
        std::cout << std::endl << "#" << queueFamilyIndex << std::endl << std::endl;
        std::cout << "Queue flags";
        for (const auto bit : {
            VK_QUEUE_GRAPHICS_BIT,
            VK_QUEUE_COMPUTE_BIT,
            VK_QUEUE_TRANSFER_BIT,
            VK_QUEUE_SPARSE_BINDING_BIT,
            VK_QUEUE_PROTECTED_BIT})
        {
            if (properties.queueFlags & bit)
            {
                printEndLn();
                std::cout << '\t' << magma::helpers::stringize(bit);
            }
        }
        printEndLn();
        printLn("Queue count", properties.queueCount);
        printLn("Timestamp valid bits", properties.timestampValidBits);
        printLn("Min image transfer granularity",
            properties.minImageTransferGranularity.width,
            properties.minImageTransferGranularity.height,
            properties.minImageTransferGranularity.depth);
#ifdef VK_USE_PLATFORM_WIN32_KHR
        // On Win32 we don't need display and visual ID
        const bool supportsPresentation = physicalDevice->getPresentationSupport(queueFamilyIndex, nullptr);
        printLn("Supports presentation", booleanString(supportsPresentation));
#endif
        ++queueFamilyIndex;
    }
}

void printDeviceMemoryTypes(magma::PhysicalDevicePtr physicalDevice)
{
    const auto properties = physicalDevice->getMemoryProperties();
    for (uint32_t i = 0; i < properties.memoryTypeCount; ++i)
    {
        const VkMemoryType& memoryType = properties.memoryTypes[i];
        printEndLn();
        std::cout << "#" << i << std::endl << std::endl;
        std::cout << "Properties";
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
                printEndLn();
                std::cout << '\t' << magma::helpers::stringize(bit);
                hasFlags = true;
            }
        }
        if (!hasFlags)
        {
            printEndLn();
            std::cout << '\t' << "---";
        }
        printEndLn();
        std::cout << "Heap index " << memoryType.heapIndex << std::endl;
    }
}

void printDeviceMemoryHeaps(magma::PhysicalDevicePtr physicalDevice)
{
    const auto properties = physicalDevice->getMemoryProperties();
    for (uint32_t i = 0; i < properties.memoryHeapCount; ++i)
    {
        const VkMemoryHeap& memoryHeap = properties.memoryHeaps[i];
        printEndLn();
        std::cout << "#" << i << std::endl << std::endl;
        std::cout << "Heap size " << memoryHeap.size << std::endl;
        std::cout << "Heap flags";
        bool hasFlags = false;
        for (const auto bit : {
            VK_MEMORY_HEAP_DEVICE_LOCAL_BIT,
            VK_MEMORY_HEAP_MULTI_INSTANCE_BIT})
        {
            if (memoryHeap.flags & bit)
            {
                printEndLn();
                std::cout << '\t' << magma::helpers::stringize(bit);
                hasFlags = true;
            }
        }
        if (!hasFlags)
        {
            printEndLn();
            std::cout << '\t' << "---";
        }
        printEndLn();
    }
}

void printExtensionFeatures(magma::PhysicalDevicePtr physicalDevice)
{
    const auto features = physicalDevice->checkExtensionFeaturesSupport();
    printEndLn();
    printLn("Device coherent memory", booleanString(features.deviceCoherentMemory));
    printLn("Advanced blend coherent operations", booleanString(features.advancedBlendCoherentOperations));
    printLn("Depth clip enable", booleanString(features.depthClipEnable));
    printLn("Extended dynamic state", booleanString(features.extendedDynamicState));
    printLn("Host query reset", booleanString(features.hostQueryReset));
    printLn("Robust image access", booleanString(features.robustImageAccess));
    printLn("Index type uint8", booleanString(features.indexTypeUint8));
    printLn("Memory priority", booleanString(features.memoryPriority));
    printLn("Pipeline creation cache control", booleanString(features.pipelineCreationCacheControl));
    printLn("Private data", booleanString(features.privateData));
    printLn("Scalar block layout", booleanString(features.scalarBlockLayout));
    printLn("Shader demote to helper invocation", booleanString(features.shaderDemoteToHelperInvocation));
    printLn("Texel buffer alignment", booleanString(features.texelBufferAlignment));
    printLn("YCbCr image arrays", booleanString(features.ycbcrImageArrays));
    printLn("Imageless framebuffer", booleanString(features.imagelessFramebuffer));
    printLn("Pipeline executable info", booleanString(features.pipelineExecutableInfo));
    printLn("Sampler YCbCr conversion", booleanString(features.samplerYcbcrConversion));
    printLn("Separate depth stencil layouts", booleanString(features.separateDepthStencilLayouts));
    printLn("Shader draw parameters", booleanString(features.shaderDrawParameters));
    printLn("Shader subgroup extended types", booleanString(features.shaderSubgroupExtendedTypes));
    printLn("Shader terminate invocation", booleanString(features.shaderTerminateInvocation));
    printLn("Timeline semaphore", booleanString(features.timelineSemaphore));
    printLn("Uniform buffer standard layout", booleanString(features.uniformBufferStandardLayout));
    printLn("Corner sampled image", booleanString(features.cornerSampledImage));
    printLn("Coverage reduction mode", booleanString(features.coverageReductionMode));
    printLn("Dedicated allocation image aliasing", booleanString(features.dedicatedAllocationImageAliasing));
    printLn("Diagnostics config", booleanString(features.diagnosticsConfig));
    printLn("Fragment shader barycentric", booleanString(features.fragmentShaderBarycentric));
    printLn("Representative fragment test", booleanString(features.representativeFragmentTest));
    printLn("Exclusive scissor", booleanString(features.exclusiveScissor));
    printLn("Image footprint", booleanString(features.imageFootprint));
    printLn("Shader streaming multiprocessor built-ins", booleanString(features.shaderSMBuiltins));
}

void print8BitStorageProperties(magma::PhysicalDevicePtr physicalDevice)
{
    MAGMA_UNUSED(physicalDevice);
#ifdef VK_KHR_8bit_storage
    const auto features = physicalDevice->get8BitStorageFeatures();
    printEndLn();
    printLn("Storage buffer 8-bit access", booleanString(features.storageBuffer8BitAccess));
    printLn("Uniform and storage buffer 8-bit access", booleanString(features.uniformAndStorageBuffer8BitAccess));
    printLn("Storage push constant 8-bit members", booleanString(features.storagePushConstant8));
#endif // VK_KHR_8bit_storage
}

void print16BitStorageProperties(magma::PhysicalDevicePtr physicalDevice)
{
    MAGMA_UNUSED(physicalDevice);
#ifdef VK_KHR_16bit_storage
    const auto features = physicalDevice->get16BitStorageFeatures();
    printEndLn();
    printLn("Storage buffer 16-bit access", booleanString(features.storageBuffer16BitAccess));
    printLn("Uniform and storage buffer 16-bit access", booleanString(features.uniformAndStorageBuffer16BitAccess));
    printLn("Storage push constant 16-bit members", booleanString(features.storagePushConstant16));
    printLn("Storage input/output 16-bit members", booleanString(features.storageInputOutput16));
#endif // VK_KHR_16bit_storage
}

void printConservativeRasterizationProperties(magma::PhysicalDevicePtr physicalDevice)
{
    MAGMA_UNUSED(physicalDevice);
#ifdef VK_EXT_conservative_rasterization
    const auto properties = physicalDevice->getConservativeRasterizationProperties();
    printEndLn();
    printLn("Primitive overestimation size", properties.primitiveOverestimationSize);
    printLn("Max extra primitive overestimation size", properties.maxExtraPrimitiveOverestimationSize);
    printLn("Extra primitive overestimation size granularity", properties.extraPrimitiveOverestimationSizeGranularity);
    printLn("Primitive underestimation", booleanString(properties.primitiveUnderestimation));
    printEndLn();
    printLn("Conservative point and line rasterization", booleanString(properties.conservativePointAndLineRasterization));
    printLn("Degenerate triangles rasterized", booleanString(properties.degenerateTrianglesRasterized));
    printLn("Degenerate lines rasterized", booleanString(properties.degenerateLinesRasterized));
    printEndLn();
    printLn("Fully covered fragment shader input variable", booleanString(properties.fullyCoveredFragmentShaderInputVariable));
    printLn("Conservative rasterization post depth coverage", booleanString(properties.conservativeRasterizationPostDepthCoverage));
#endif // VK_EXT_conservative_rasterization
}

void printLineRasterizationProperties(magma::PhysicalDevicePtr physicalDevice)
{
    MAGMA_UNUSED(physicalDevice);
#ifdef VK_EXT_line_rasterization
    const auto features = physicalDevice->getLineRasterizationFeatures();
    printEndLn();
    printLn("Rectangular lines", booleanString(features.rectangularLines));
    printLn("Bresenham lines", booleanString(features.bresenhamLines));
    printLn("Smooth lines", booleanString(features.smoothLines));
    printEndLn();
    printLn("Stippled rectangular lines", booleanString(features.stippledRectangularLines));
    printLn("Stippled Bresenham lines", booleanString(features.stippledBresenhamLines));
    printLn("Stippled smooth lines", booleanString(features.stippledSmoothLines));
    const auto& properties = physicalDevice->getLineRasterizationProperties();
    printEndLn();
    printLn("Line sub-pixel precision bits", properties.lineSubPixelPrecisionBits);
#endif // VK_EXT_line_rasterization
}

void printShaderCoreProperties(magma::PhysicalDevicePtr physicalDevice)
{
    MAGMA_UNUSED(physicalDevice);
#ifdef VK_AMD_shader_core_properties
    const auto properties = physicalDevice->getShaderCoreProperties();
    printEndLn();
    printLn("Shader engine count", properties.shaderEngineCount);
    printLn("Shader arrays per engine count", properties.shaderArraysPerEngineCount);
    printLn("Compute units per shader array", properties.computeUnitsPerShaderArray);
    printLn("SIMD per compute unit", properties.simdPerComputeUnit);
    printEndLn();
    printLn("Wavefronts per SIMD", properties.wavefrontsPerSimd);
    printLn("Wavefront size", properties.wavefrontSize);
    printEndLn();
    printLn("SGPRs per SIMD", properties.sgprsPerSimd);
    printLn("Min SGPR allocations", properties.minSgprAllocation);
    printLn("Max SGPR allocations", properties.maxSgprAllocation);
    printLn("SGPR allocation granularity", properties.sgprAllocationGranularity);
    printEndLn();
    printLn("VGPRs per SIMD", properties.vgprsPerSimd);
    printLn("Min VGPR allocation", properties.minVgprAllocation);
    printLn("Max VGPR allocation", properties.maxVgprAllocation);
    printLn("VGPR allocation granularity", properties.vgprAllocationGranularity);
#endif // VK_AMD_shader_core_properties
}

void printExtendedShaderCoreProperties(magma::PhysicalDevicePtr physicalDevice)
{
    MAGMA_UNUSED(physicalDevice);
#ifdef VK_AMD_shader_core_properties2
    const auto properties = physicalDevice->getShaderCoreProperties2();
    printEndLn();
    printLn("Active compute unit count", properties.activeComputeUnitCount);
#endif
}

void printMeshShaderProperties(magma::PhysicalDevicePtr physicalDevice)
{
    MAGMA_UNUSED(physicalDevice);
#ifdef VK_NV_mesh_shader
    const auto features = physicalDevice->getMeshShaderFeatures();
    printEndLn();
    printLn("Task shader", booleanString(features.taskShader));
    printLn("Mesh shader", booleanString(features.meshShader));
    const auto properties = physicalDevice->getMeshShaderProperties();
    printEndLn();
    printLn("Max draw mesh task count", uint32String(properties.maxDrawMeshTasksCount));
    printEndLn();
    printLn("Max task work group invocations", properties.maxTaskWorkGroupInvocations);
    printLn("Max task work group size",
        properties.maxTaskWorkGroupSize[0],
        properties.maxTaskWorkGroupSize[1],
        properties.maxTaskWorkGroupSize[2]);
    printLn("Max task total memory size", uint32String(properties.maxTaskTotalMemorySize));
    printLn("Max task output count", uint32String(properties.maxTaskOutputCount));
    printEndLn();
    printLn("Max mesh work group invocations", properties.maxMeshWorkGroupInvocations);
    printLn("Max mesh work group size",
        properties.maxMeshWorkGroupSize[0],
        properties.maxMeshWorkGroupSize[1],
        properties.maxMeshWorkGroupSize[2]);
    printLn("Max mesh total memory size", uint32String(properties.maxMeshTotalMemorySize));
    printLn("Max mesh output vertices", properties.maxMeshOutputVertices);
    printLn("Max mesh output primitives", properties.maxMeshOutputPrimitives);
    printLn("Max mesh multiview view count", properties.maxMeshMultiviewViewCount);
    printEndLn();
    printLn("Mesh output per vertex granularity", properties.meshOutputPerVertexGranularity);
    printLn("Mesh output per primitive granularity", properties.meshOutputPerPrimitiveGranularity);
#endif // VK_NV_mesh_shader
}

void printShaderSMBuiltinsProperties(magma::PhysicalDevicePtr physicalDevice)
{
#ifdef VK_NV_shader_sm_builtins
    const auto properties = physicalDevice->getShaderSMBuiltinsProperties();
    printEndLn();
    printLn("Shader streaming multiprocessor count", properties.shaderSMCount);
    printLn("Shader warps per streaming multiprocessor", properties.shaderWarpsPerSM);
#endif // VK_NV_shader_sm_builtins
}

void printInlineUniformBlockProperties(magma::PhysicalDevicePtr physicalDevice)
{
    MAGMA_UNUSED(physicalDevice);
#ifdef VK_EXT_inline_uniform_block
    const auto features = physicalDevice->getInlineUniformBlockFeatures();
    printEndLn();
    printLn("Inline uniform block", booleanString(features.inlineUniformBlock));
    printLn("Descriptor binding inline uniform block update after bind", booleanString(features.descriptorBindingInlineUniformBlockUpdateAfterBind));
    const auto properties = physicalDevice->getInlineUniformBlockProperties();
    printEndLn();
    printLn("Max inline uniform block size", properties.maxInlineUniformBlockSize);
    printLn("Max per stage descriptor inline uniform blocks", properties.maxPerStageDescriptorInlineUniformBlocks);
    printLn("Max per stage descriptor update after bind inline uniform blocks", properties.maxPerStageDescriptorUpdateAfterBindInlineUniformBlocks);
    printLn("Max descriptor set inline uniform blocks", properties.maxDescriptorSetInlineUniformBlocks);
    printLn("Max descriptor set update after bind inline uniform blocks", properties.maxDescriptorSetUpdateAfterBindInlineUniformBlocks);
#endif // VK_EXT_inline_uniform_block
}

void printDescriptorIndexingProperties(magma::PhysicalDevicePtr physicalDevice)
{
    MAGMA_UNUSED(physicalDevice);
#ifdef VK_EXT_descriptor_indexing
    const auto features = physicalDevice->getDescriptorIndexingFeatures();
    printEndLn();
    printLn("Shader input attachment array dynamic indexing", booleanString(features.shaderInputAttachmentArrayDynamicIndexing));
    printLn("Shader uniform texel buffer array dynamic indexing", booleanString(features.shaderUniformTexelBufferArrayDynamicIndexing));
    printLn("Shader storage texel buffer array dynamic indexing", booleanString(features.shaderStorageTexelBufferArrayDynamicIndexing));
    printLn("Shader uniform buffer array non-uniform indexing", booleanString(features.shaderUniformBufferArrayNonUniformIndexing));
    printLn("Shader sampled image array non-uniform indexing", booleanString(features.shaderSampledImageArrayNonUniformIndexing));
    printLn("Shader storage buffer array non-uniform indexing", booleanString(features.shaderStorageBufferArrayNonUniformIndexing));
    printLn("Shader storage image array non-uniform indexing", booleanString(features.shaderStorageImageArrayNonUniformIndexing));
    printLn("Shader input attachment array non-uniform indexing", booleanString(features.shaderInputAttachmentArrayNonUniformIndexing));
    printLn("Shader uniform texel buffer array non-uniform indexing", booleanString(features.shaderUniformTexelBufferArrayNonUniformIndexing));
    printLn("Shader storage texel buffer array non-uniform indexing", booleanString(features.shaderStorageTexelBufferArrayNonUniformIndexing));
    printEndLn();
    printLn("Descriptor binding uniform buffer update after bind", booleanString(features.descriptorBindingUniformBufferUpdateAfterBind));
    printLn("Descriptor binding sampled image update after bind", booleanString(features.descriptorBindingSampledImageUpdateAfterBind));
    printLn("Descriptor binding storage image update after bind", booleanString(features.descriptorBindingStorageImageUpdateAfterBind));
    printLn("Descriptor binding storage buffer update after bind", booleanString(features.descriptorBindingStorageBufferUpdateAfterBind));
    printLn("Descriptor binding uniform texel buffer update after bind", booleanString(features.descriptorBindingUniformTexelBufferUpdateAfterBind));
    printLn("Descriptor binding storage texel buffer update after bind", booleanString(features.descriptorBindingStorageTexelBufferUpdateAfterBind));
    printLn("Descriptor binding update unused while pending", booleanString(features.descriptorBindingUpdateUnusedWhilePending));
    printLn("Descriptor binding partially bound", booleanString(features.descriptorBindingPartiallyBound));
    printLn("Descriptor binding variable descriptor count", booleanString(features.descriptorBindingVariableDescriptorCount));
    printEndLn();
    printLn("Runtime descriptor array", booleanString(features.runtimeDescriptorArray));
    const auto properties = physicalDevice->getDescriptorIndexingProperties();
    printEndLn();
    printLn("Max update after bind descriptors in all pools", uint32String(properties.maxUpdateAfterBindDescriptorsInAllPools));
    printEndLn();
    printLn("Shader uniform buffer array non-uniform indexing native", booleanString(properties.shaderUniformBufferArrayNonUniformIndexingNative));
    printLn("Shader sampled image array non-uniform indexing native", booleanString(properties.shaderSampledImageArrayNonUniformIndexingNative));
    printLn("Shader storage buffer array non-uniform indexing native", booleanString(properties.shaderStorageBufferArrayNonUniformIndexingNative));
    printLn("Shader storage image array non-uniform indexing native", booleanString(properties.shaderStorageImageArrayNonUniformIndexingNative));
    printLn("Shader input attachment array non-uniform indexing native", booleanString(properties.shaderInputAttachmentArrayNonUniformIndexingNative));
    printEndLn();
    printLn("Robust buffer access update after bind", booleanString(properties.robustBufferAccessUpdateAfterBind));
    printLn("Quad divergent implicit LOD", booleanString(properties.quadDivergentImplicitLod));
    printEndLn();
    printLn("Max per stage descriptor update after bind samplers", properties.maxPerStageDescriptorUpdateAfterBindSamplers);
    printLn("Max per stage descriptor update after bind uniform buffers", properties.maxPerStageDescriptorUpdateAfterBindUniformBuffers);
    printLn("Max per stage descriptor update after bind storage buffers", properties.maxPerStageDescriptorUpdateAfterBindStorageBuffers);
    printLn("Max per stage descriptor update after bind sampled images", properties.maxPerStageDescriptorUpdateAfterBindSampledImages);
    printLn("Max per stage descriptor update after bind storage images", properties.maxPerStageDescriptorUpdateAfterBindStorageImages);
    printLn("Max per stage descriptor update after bind input attachments", properties.maxPerStageDescriptorUpdateAfterBindInputAttachments);
    printLn("Max per stage update after bind resources", uint32String(properties.maxPerStageUpdateAfterBindResources));
    printEndLn();
    printLn("Max descriptor set update after bind samplers", properties.maxDescriptorSetUpdateAfterBindSamplers);
    printLn("Max descriptor set update after bind uniform buffers", properties.maxDescriptorSetUpdateAfterBindUniformBuffers);
    printLn("Max descriptor set update after bind uniform buffers dynamic", properties.maxDescriptorSetUpdateAfterBindUniformBuffersDynamic);
    printLn("Max descriptor set update after bind storage buffers", properties.maxDescriptorSetUpdateAfterBindStorageBuffers);
    printLn("Max descriptor set update after bind storage buffers dynamic", properties.maxDescriptorSetUpdateAfterBindStorageBuffersDynamic);
    printLn("Max descriptor set update after bind sampled images", properties.maxDescriptorSetUpdateAfterBindSampledImages);
    printLn("Max descriptor set update after bind storage images", properties.maxDescriptorSetUpdateAfterBindStorageImages);
    printLn("Max descriptor set update after bind input attachments", properties.maxDescriptorSetUpdateAfterBindInputAttachments);
#endif // VK_EXT_blend_operation_advanced
}

void printConditionalRenderingProperties(magma::PhysicalDevicePtr physicalDevice)
{
    MAGMA_UNUSED(physicalDevice);
#ifdef VK_EXT_conditional_rendering
    const auto features = physicalDevice->getConditionalRenderingFeatures();
    printEndLn();
    printLn("Conditional rendering", booleanString(features.conditionalRendering));
    printLn("Inherited conditional rendering", booleanString(features.inheritedConditionalRendering));
#endif // VK_EXT_conditional_rendering
}

void printTransformFeedbackProperties(magma::PhysicalDevicePtr physicalDevice)
{
    MAGMA_UNUSED(physicalDevice);
#ifdef VK_EXT_transform_feedback
    const auto features = physicalDevice->getTransformFeedbackFeatures();
    printEndLn();
    printLn("Transform feedback", booleanString(features.transformFeedback));
    printLn("Geometry streams", booleanString(features.geometryStreams));
    const auto properties = physicalDevice->getTransformFeedbackProperties();
    printEndLn();
    printLn("Max transform feedback streams", properties.maxTransformFeedbackStreams);
    printLn("Max transform feedback buffers", properties.maxTransformFeedbackBuffers);
    printLn("Max transform feedback buffer size", properties.maxTransformFeedbackBufferSize);
    printLn("Max transform feedback stream data size", properties.maxTransformFeedbackStreamDataSize);
    printLn("Max transform feedback buffer data size", properties.maxTransformFeedbackBufferDataSize);
    printLn("Max transform feedback buffer data stride", properties.maxTransformFeedbackBufferDataStride);
    printEndLn();
    printLn("Transform feedback queries", booleanString(properties.transformFeedbackQueries));
    printLn("Transform feedback streams lines triangles", booleanString(properties.transformFeedbackStreamsLinesTriangles));
    printLn("Transform feedback rasterization stream select", booleanString(properties.transformFeedbackRasterizationStreamSelect));
    printLn("Transform feedback draw", booleanString(properties.transformFeedbackDraw));
#endif // VK_EXT_transform_feedback
}

void printImageShadingRateProperties(magma::PhysicalDevicePtr physicalDevice)
{
    MAGMA_UNUSED(physicalDevice);
#ifdef VK_NV_shading_rate_image
    const auto features = physicalDevice->getShadingRateImageFeatures();
    printEndLn();
    printLn("Shading rate image", booleanString(features.shadingRateImage));
    printLn("Shading rate coarse sample order", booleanString(features.shadingRateCoarseSampleOrder));
#endif // VK_NV_shading_rate_image
}

void printMultiviewProperties(magma::PhysicalDevicePtr physicalDevice)
{
#ifdef VK_KHR_multiview
    const auto features = physicalDevice->getMultiviewFeatures();
    printEndLn();
    printLn("Multiview", booleanString(features.multiview));
    printLn("Multiview geometry shader", booleanString(features.multiviewGeometryShader));
    printLn("Multiview tessellation shader", booleanString(features.multiviewTessellationShader));
    const auto properties = physicalDevice->getMultiviewProperties();
    printEndLn();
    printLn("Max multiview view count", properties.maxMultiviewViewCount);
    printLn("Max multiview instance index", properties.maxMultiviewInstanceIndex);
#endif // VK_KHR_multiview
}

void printAdvancedBlendOperationProperties(magma::PhysicalDevicePtr physicalDevice)
{
    MAGMA_UNUSED(physicalDevice);
#ifdef VK_EXT_blend_operation_advanced
    const auto properties = physicalDevice->getBlendOperationAdvancedProperties();
    printEndLn();
    printLn("Advanced blend max color attachments", properties.advancedBlendMaxColorAttachments);
    printLn("Advanced blend independent blend", booleanString(properties.advancedBlendIndependentBlend));
    printLn("Advanced blend non-premultiplied source color", booleanString(properties.advancedBlendNonPremultipliedSrcColor));
    printLn("Advanced blend non-premultiplied dest color", booleanString(properties.advancedBlendNonPremultipliedDstColor));
    printLn("Advanced blend correlated overlap", booleanString(properties.advancedBlendCorrelatedOverlap));
    printLn("Advanced blend all operations", booleanString(properties.advancedBlendAllOperations));
#endif // VK_EXT_blend_operation_advanced
}

void printRayTracingProperties(magma::PhysicalDevicePtr physicalDevice)
{
    MAGMA_UNUSED(physicalDevice);
#ifdef VK_NV_ray_tracing
    const auto properties = physicalDevice->getRayTracingProperties();
    printEndLn();
    printLn("Shader group handle size", properties.shaderGroupHandleSize);
    printLn("Max recursion depth", properties.maxRecursionDepth);
    printLn("Max shader group stride", properties.maxShaderGroupStride);
    printLn("Shader group base alignment", properties.shaderGroupBaseAlignment);
    printEndLn();
    printLn("Max geometry count", properties.maxGeometryCount);
    printLn("Max instance count", properties.maxInstanceCount);
    printLn("Max triangle count", properties.maxTriangleCount);
    printLn("Max descriptor set acceleration structures", properties.maxDescriptorSetAccelerationStructures);
#endif // VK_NV_ray_tracing
}

void printExtensions(std::shared_ptr<magma::Extensions> extensions)
{
    printEndLn();
    printLn("Name", "Specification");
    extensions->forEach(
        [](const std::string& extensionName, uint32_t specVersion)
        {
            printLn(extensionName.c_str(), specVersion);
        });
}

void printInstanceLayers(std::shared_ptr<magma::InstanceLayers> instanceLayers)
{
    printEndLn();
    std::cout << std::setw(width) << std::left << "Name"
        << std::setw(15) << std::left << "Specification" << "Description" << std::endl;
    instanceLayers->forEach(
        [](const VkLayerProperties& properties)
        {
            std::cout << std::setw(width) << std::left << properties.layerName
                << std::setw(15) << std::left << apiVersionString(properties.specVersion)
                << properties.description << std::endl;
        });
}

magma::InstancePtr createInstance(std::shared_ptr<magma::InstanceLayers> instanceLayers)
{
    std::vector<const char*> layerNames;
#ifdef _DEBUG
    if (instanceLayers->KHRONOS_validation)
        layerNames.push_back("VK_LAYER_KHRONOS_validation");
    else if (instanceLayers->LUNARG_standard_validation)
        layerNames.push_back("VK_LAYER_LUNARG_standard_validation");
#endif // _DEBUG
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
    std::cout << "Vulkan GPU Caps Viewer [Version 1.1]" << std::endl;
    std::cout << "(c) 2018-2021 Victor Coda." << std::endl;

    auto instanceLayers = std::make_shared<magma::InstanceLayers>();
    auto instance = createInstance(instanceLayers);
    if (!instance)
        return -1;
    auto instanceExtensions = std::make_shared<magma::InstanceExtensions>();
    printHeading("Instance Extensions");
    setFieldWidth(45);
    printExtensions(instanceExtensions);
    printHeading("Instance Layers");
    setFieldWidth(40);
    printInstanceLayers(std::move(instanceLayers));
    if (instanceExtensions->KHR_device_group_creation)
    {
        printHeading("Device Groups");
        setFieldWidth(30);
        printDeviceGroups(instance);
    }
    const uint32_t physicalDeviceCount = instance->countPhysicalDevices();
    for (uint32_t deviceId = 0; deviceId < physicalDeviceCount; ++deviceId)
    {
        magma::PhysicalDevicePtr physicalDevice = instance->getPhysicalDevice(deviceId);
        setFieldWidth(20);
        printDeviceProperties(physicalDevice, deviceId);
        auto deviceExtensions = std::make_shared<magma::PhysicalDeviceExtensions>(physicalDevice);
        if (deviceExtensions->KHR_driver_properties)
        {
            printHeading("Driver Properties");
            setFieldWidth(20);
            printDriverProperties(physicalDevice);
        }
        printHeading("Device Features");
        setFieldWidth(45);
        printDeviceFeatures(physicalDevice);
        printHeading("Device Limits");
        setFieldWidth(55);
        printDeviceLimits(physicalDevice);
        printHeading("Queue Family");
        setFieldWidth(40);
        printQueueFamilyProperties(physicalDevice);
        printHeading("Device Memory Types");
        printDeviceMemoryTypes(physicalDevice);
        printHeading("Device Memory Heaps");
        printDeviceMemoryHeaps(physicalDevice);
        if (instanceExtensions->KHR_get_physical_device_properties2)
        {
            printHeading("Extension Features");
            setFieldWidth(45);
            printExtensionFeatures(physicalDevice);
        }
        if (deviceExtensions->KHR_8bit_storage)
        {
            printHeading("8-bit Storage");
            setFieldWidth(45);
            print8BitStorageProperties(physicalDevice);
        }
        if (deviceExtensions->KHR_16bit_storage)
        {
            printHeading("16-bit Storage");
            setFieldWidth(45);
            print16BitStorageProperties(physicalDevice);
        }
        if (deviceExtensions->EXT_conservative_rasterization)
        {
            printHeading("Conservative Rasterization");
            setFieldWidth(50);
            printConservativeRasterizationProperties(physicalDevice);
        }
        if (deviceExtensions->EXT_line_rasterization)
        {
            printHeading("Line Rasterization");
            setFieldWidth(35);
            printLineRasterizationProperties(physicalDevice);
        }
        if (deviceExtensions->AMD_shader_core_properties)
        {
            printHeading("Shader Core");
            setFieldWidth(35);
            printShaderCoreProperties(physicalDevice);
            if (deviceExtensions->AMD_shader_core_properties2)
                printExtendedShaderCoreProperties(physicalDevice);
        }
        if (deviceExtensions->NV_mesh_shader)
        {
            printHeading("Mesh Shader");
            setFieldWidth(40);
            printMeshShaderProperties(physicalDevice);
        }
        if (deviceExtensions->NV_shader_sm_builtins)
        {
            printHeading("Shader Streaming Multiprocessors");
            setFieldWidth(45);
            printShaderSMBuiltinsProperties(physicalDevice);
        }
        if (deviceExtensions->EXT_inline_uniform_block)
        {
            printHeading("Inline Uniform Block");
            setFieldWidth(65);
            printInlineUniformBlockProperties(physicalDevice);
        }
        if (deviceExtensions->EXT_descriptor_indexing)
        {
            printHeading("Descriptor Indexing");
            setFieldWidth(65);
            printDescriptorIndexingProperties(physicalDevice);
        }
        if (deviceExtensions->EXT_conditional_rendering)
        {
            printHeading("Conditional Rendering");
            setFieldWidth(35);
            printConditionalRenderingProperties(physicalDevice);
        }
        if (deviceExtensions->EXT_transform_feedback)
        {
            printHeading("Transform Feedback");
            setFieldWidth(50);
            printTransformFeedbackProperties(physicalDevice);
        }
        if (deviceExtensions->NV_shading_rate_image)
        {
            printHeading("Image Shading Rate");
            setFieldWidth(35);
            printImageShadingRateProperties(physicalDevice);
        }
        if (deviceExtensions->KHR_multiview)
        {
            printHeading("Multi View");
            setFieldWidth(35);
            printMultiviewProperties(physicalDevice);
        }
        if (deviceExtensions->EXT_blend_operation_advanced)
        {
            printHeading("Advanced Blend Operation");
            setFieldWidth(50);
            printAdvancedBlendOperationProperties(physicalDevice);
        }
        if (deviceExtensions->NV_ray_tracing)
        {
            printHeading("Ray Tracing");
            setFieldWidth(45);
            printRayTracingProperties(physicalDevice);
        }
        printHeading("Device Extensions");
        setFieldWidth(45);
        printExtensions(deviceExtensions);
        if (physicalDeviceCount > 1 && deviceId < physicalDeviceCount - 1)
        {
            std::cout << "Print any key for the next device";
            std::cin.get();
        }
    }
    return 0;
}
