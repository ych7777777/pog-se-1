#include "render_pipeline.hpp"

framework::uint32 framework::runtime::render::render_pipelines::index(const std::string& pipeline, const std::string& name) const noexcept
{
	return mPipelines.at(pipeline).root_signature_info.index(name);
}

bool framework::runtime::render::render_pipelines::empty() const noexcept
{
	return mPipelines.empty();
}

const framework::runtime::render::render_pipeline& framework::runtime::render::render_pipelines::at(
	const std::string& name) const
{
	return mPipelines.at(name);
}

framework::runtime::render::render_pipeline& framework::runtime::render::render_pipelines::operator[](const std::string& name)
{
	return mPipelines[name];
}
