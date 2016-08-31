#pragma once


#include <windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include "d3dx12.h"

#include "ConstantBufferPerObject.h"

#include "text.h"
#include "wic.h"
#include "camera.h"


#ifndef DX12_ENGINE_WINDOW_H
#define DX12_ENGINE_WINDOW_H

// this will only call release if an object exists (prevents exceptions calling release on non existant objects)
#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }

#define DEFAULT_FRAME_BUFFER_COUNT 3

namespace DX12Engine {

	class Window {
	private:

		// Handle to the window
		HWND hwnd = NULL;

		// name of the window (not the title)
		LPCTSTR WindowName = L"Hypernova Core";

		// title of the window
		LPCTSTR WindowTitle = L"Hypernova";

		// width and height of the window
		int Width = 1200;
		int Height = 960;

		// is window full screen?
		bool FullScreen = false;

		// we will exit the program when this becomes false
		bool Running = true;

		bool Tracking = true;

		// direct3d stuff
		const int frameBufferCount = 3; // number of buffers we want, 2 for double buffering, 3 for tripple buffering

		ID3D12Device* device; // direct3d device

		IDXGISwapChain3* swapChain; // swapchain used to switch between render targets

		ID3D12CommandQueue* commandQueue; // container for command lists

		ID3D12DescriptorHeap* rtvDescriptorHeap; // a descriptor heap to hold resources like the render targets

		ID3D12Resource* renderTargets[DEFAULT_FRAME_BUFFER_COUNT]; // number of render targets equal to buffer count

		ID3D12CommandAllocator* commandAllocator[DEFAULT_FRAME_BUFFER_COUNT]; // we want enough allocators for each buffer * number of threads (we only have one thread)


		ID3D12Fence* fence[DEFAULT_FRAME_BUFFER_COUNT];    // an object that is locked while our command list is being executed by the gpu. We need as many 
												 //as we have allocators (more if we want to know when the gpu is finished with an asset)

		HANDLE fenceEvent; // a handle to an event when our fence is unlocked by the gpu

		UINT64 fenceValue[DEFAULT_FRAME_BUFFER_COUNT]; // this value is incremented each frame. each fence will have its own value

		int frameIndex; // current rtv we are on

		int rtvDescriptorSize; // size of the rtv descriptor on the device (all front and back buffers will be the same size)
							   // function declarations


		ID3D12PipelineState* pipelineStateObject; // pso containing a pipeline state
		ID3D12PipelineState* textPSO; // pso containing a pipeline state

		ID3D12RootSignature* rootSignature; // root signature defines data shaders will access

		D3D12_VIEWPORT viewport; // area that output from rasterizer will be stretched to.
		D3D12_RECT scissorRect; // the area to draw in. pixels outside that area will not be drawn onto

		ID3D12Resource* vertexBuffer; // a default buffer in GPU memory that we will load vertex data for our triangle into
		ID3D12Resource* indexBuffer; // a default buffer in GPU memory that we will load index data for our triangle into
		ID3D12Resource* depthStencilBuffer; // This is the memory for our depth buffer. it will also be used for a stencil buffer in a later tutorial
		ID3D12Resource* constantBufferUploadHeaps[DEFAULT_FRAME_BUFFER_COUNT]; // this is the memory on the gpu where constant buffers for each frame will be placed
		ID3D12Resource* textureBuffer; // the resource heap containing our texture
		ID3D12Resource* textureBufferUploadHeap;
		ID3D12Resource* textVertexBuffer[DEFAULT_FRAME_BUFFER_COUNT];

		D3D12_VERTEX_BUFFER_VIEW vertexBufferView; // a structure containing a pointer to the vertex data in gpu memory
												   // the total size of the buffer, and the size of each element (vertex)
		D3D12_VERTEX_BUFFER_VIEW textVertexBufferView[DEFAULT_FRAME_BUFFER_COUNT]; // a view for our text vertex buffer

		D3D12_INDEX_BUFFER_VIEW indexBufferView; // a structure holding information about the index buffer

		ID3D12DescriptorHeap* dsDescriptorHeap; // This is a heap for our depth/stencil buffer descriptor


												// Constant buffers must be 256-byte aligned which has to do with constant reads on the GPU.
												// We are only able to read at 256 byte intervals from the start of a resource heap, so we will
												// make sure that we add padding between the two constant buffers in the heap (one for cube1 and one for cube2)
												// Another way to do this would be to add a float array in the constant buffer structure for padding. In this case
												// we would need to add a float padding[50]; after the wvpMat variable. This would align our structure to 256 bytes (4 bytes per float)
												// The reason i didn't go with this way, was because there would actually be wasted cpu cycles when memcpy our constant
												// buffer data to the gpu virtual address. currently we memcpy the size of our structure, which is 16 bytes here, but if we
												// were to add the padding array, we would memcpy 64 bytes if we memcpy the size of our structure, which is 50 wasted bytes
												// being copied.

		int ConstantBufferPerObjectAlignedSize = (sizeof(ConstantBufferPerObject) + 255) & ~255;

		UINT8* cbvGPUAddress[DEFAULT_FRAME_BUFFER_COUNT]; // this is a pointer to each of the constant buffer resource heaps
		UINT8* textVBGPUAddress[DEFAULT_FRAME_BUFFER_COUNT]; // this is a pointer to each of the text constant buffers

		int numCubeIndices; // the number of indices to draw the cube


		ID3D12DescriptorHeap* mainDescriptorHeap;
		UINT srvHandleSize;

		Font arialFont; // this will store our arial font information

		int maxNumTextCharacters = 1024; // the maximum number of characters you can render during a frame. This is just used to make sure
										 // there is enough memory allocated for the text vertex buffer each frame


												   // create an instance of timer


		int fpscounter = 0;

		Camera camera;


#define MAX_KEYS 65535
		bool keyDown[MAX_KEYS];

		RECT rcClip;           // new area for ClipCursor
		RECT rcOldClip;        // previous area for ClipCursor
		
	public:
		ID3D12GraphicsCommandList* commandList; // a command list we can record commands into, then execute them to render the frame
		
	private:
		// create a window
		bool InitializeWindow(HINSTANCE hInstance,
			int ShowWnd,
			bool fullscreen);

		bool InitD3D(); // initializes direct3d 12

		void UpdatePipeline();

		void Cleanup();

	public:
		
		Window();
		~Window();

		void update();
		void WaitForPreviousFrame();

		void RenderText(Font font, std::wstring text, XMFLOAT2 pos, XMFLOAT2 scale = XMFLOAT2(1.0f, 1.0f), XMFLOAT2 padding = XMFLOAT2(0.5f, 0.0f), XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));




	};

}

#endif

