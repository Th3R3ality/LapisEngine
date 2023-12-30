#include "InternalTypes.h"
#include "Backend.h"

namespace Lapis
{
	InternalLapisCommand::InternalLapisCommand(LapisCommand lapisCommand, UINT startVertexLocation)
	: vertexCount(lapisCommand.vertexCount),
		startVertexLocation(startVertexLocation),
		topology(lapisCommand.topology),
		transform(lapisCommand.transform),
		material(Backend::builtinMaterials[lapisCommand.materialName])
	{}
}