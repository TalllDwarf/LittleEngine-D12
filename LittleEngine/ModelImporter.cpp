#include "ModelImporter.h"



ModelImporter::ModelImporter()
{
}


ModelImporter::~ModelImporter()
{
}

void ModelImporter::Init(ID3D12Device * device, ID3D12GraphicsCommandList * commandList)
{
	m_pDevice = device;
	m_pCommandList = commandList;
}

bool ModelImporter::OpenFile(const char * objPath)
{
	//Set the pointers to the begining of the array
	std::vector<DirectX::XMFLOAT2> textureCoord;
	std::vector<DirectX::XMFLOAT3> normals;

	FILE * file;
	errno_t err;
	err = fopen_s(&file, objPath, "r");

	if (err == 0)
	{
		while (true)
		{
			char line[128];
			//Get first string of the line
			int res = fscanf_s(file, "%s", line, _countof(line));
			if (res == EOF)
				break;

			//if the line contains a vertex
			if (strcmp(line, "v") == 0)
			{
				Vertex vertex(0, 0, 0, -1, -1, 0.0f, 0.0f, 0.0f);
				fscanf_s(file, "%f %f %f\n", &vertex.pos.x, &vertex.pos.y, &vertex.pos.z);
				vVector.push_back(vertex);
			}
			//If the line contains a texturecoord
			else if (strcmp(line, "vt") == 0)
			{
				DirectX::XMFLOAT2 texCoord;
				fscanf_s(file, "%f %f/n", &texCoord.x, &texCoord.y);
				textureCoord.push_back(texCoord);
			}
			else if (strcmp(line, "vn") == 0)
			{
				DirectX::XMFLOAT3 normal;
				fscanf_s(file, "%f %f %f/n", &normal.x, &normal.y, &normal.z);
				normals.push_back(normal);
			}
			//if the line contains a face
			else if (strcmp(line, "f") == 0)
			{
				int vIndex[3], texIndex[3], normalIndex[3];

				fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vIndex[0], &texIndex[0], &normalIndex[0],
					&vIndex[1], &texIndex[1], &normalIndex[1], &vIndex[2], &texIndex[2], &normalIndex[2]);

				//Check if we have this vertex with a different texture
				for (int i = 0; i < 3; i++)
				{
					if (vVector[(vIndex[i] - 1)].texCoord.x == -1 || (vVector[(vIndex[i] - 1)].texCoord.x == textureCoord[(texIndex[i] - 1)].x && vVector[(vIndex[i] - 1)].texCoord.y == textureCoord[(texIndex[i] - 1)].y))
					{
						vVector[(vIndex[i] - 1)].texCoord = textureCoord[(texIndex[i] - 1)];
						vVector[(vIndex[i] - 1)].normal = normals[(normalIndex[i] - 1)];
						iVector.push_back((DWORD)vIndex[i] - 1);
					}
					else
					{
						Vertex current = vVector[(vIndex[i] - 1)];
						current.texCoord = textureCoord[(texIndex[i] - 1)];
						current.normal = normals[(normalIndex[i] - 1)];

						vVector.push_back(current);
						iVector.push_back((DWORD)(vVector.size() - 1));
					}
				}
			}
		}
		fclose(file);

		numOfIndecies = iVector.size();
		vBufferSize = vVector.size() * sizeof(Vertex);
		iBufferSize = iVector.size() * sizeof(DWORD);

		vList = reinterpret_cast<BYTE*>(&vVector[0]);
		iList = reinterpret_cast<BYTE*>(&iVector[0]);

		CommitResource();
	}
	else
	{
		return false;
	}
	
	return true;
}

void ModelImporter::SetBuffers()
{
	m_pCommandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
	m_pCommandList->IASetIndexBuffer(&m_indexBufferView);
}

void ModelImporter::CommitResource()
{
	m_pDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(vBufferSize), D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&m_pVertexBuffer));

	m_pVertexBuffer->SetName(L"Vertex Buffer Resource Heap");

	ID3D12Resource *pVertexBufferUploadHeap;

	m_pDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(vBufferSize), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&pVertexBufferUploadHeap));
	pVertexBufferUploadHeap->SetName(L"Vertex Buffer Upload Resource Heap");

	D3D12_SUBRESOURCE_DATA vertexData = {};
	vertexData.pData = GetVList(); //reinterpret_cast<BYTE*>(vList);
	vertexData.RowPitch = vBufferSize;
	vertexData.SlicePitch = vBufferSize;


	UpdateSubresources(m_pCommandList, m_pVertexBuffer, pVertexBufferUploadHeap, 0, 0, 1, &vertexData);

	m_pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_pVertexBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

	m_pDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(iBufferSize), D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&m_pIndexBuffer));

	m_pIndexBuffer->SetName(L"Index Buffer Resource Heap");

	ID3D12Resource *pIBufferUploadHeap;
	m_pDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(iBufferSize), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&pIBufferUploadHeap));
	pIBufferUploadHeap->SetName(L"Index Buffer Upload Resource Heap");

	D3D12_SUBRESOURCE_DATA indexData = {};
	indexData.pData = GetIList(); //reinterpret_cast<BYTE*>(iList);
	indexData.RowPitch = iBufferSize;
	indexData.SlicePitch = iBufferSize;

	UpdateSubresources(m_pCommandList, m_pIndexBuffer, pIBufferUploadHeap, 0, 0, 1, &indexData);

	m_pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_pIndexBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

	m_vertexBufferView.BufferLocation = m_pVertexBuffer->GetGPUVirtualAddress();
	m_vertexBufferView.StrideInBytes = sizeof(Vertex);
	m_vertexBufferView.SizeInBytes = vBufferSize;

	m_indexBufferView.BufferLocation = m_pIndexBuffer->GetGPUVirtualAddress();
	m_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	m_indexBufferView.SizeInBytes = iBufferSize;
}

void ModelImporter::CleanUp()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
}

BYTE * ModelImporter::GetVList()
{
	return vList;
}

BYTE * ModelImporter::GetIList()
{
	return iList;
}

int ModelImporter::GetIndeciesCount()
{
	return numOfIndecies;
}