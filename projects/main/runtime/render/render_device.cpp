#include "render_device.hpp"

framework::runtime::render::render_device::render_device(const D3D_FEATURE_LEVEL& level)
{
	mDevice = wrapper::directx12::device::create(level);
	mFence = wrapper::directx12::fence::create(mDevice, 0);

	mMainQueue = wrapper::directx12::command_queue::create(mDevice);
}

void framework::runtime::render::render_device::wait() const
{
	mMainQueue.wait(mFence);
}

wrapper::directx12::command_queue framework::runtime::render::render_device::queue() const noexcept
{
	return mMainQueue;
}

wrapper::directx12::device framework::runtime::render::render_device::device() const noexcept
{
	return mDevice;
}

wrapper::directx12::fence framework::runtime::render::render_device::fence() const noexcept
{
	return mFence;
}
