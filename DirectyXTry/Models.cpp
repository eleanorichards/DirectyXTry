#include "Models.h"

Models::Models()
{
	m_vertexBuffer = 0;
	m_instanceBuffer = 0;
	m_Texture = 0;
	m_model = 0;
}

Models::Models(const Models &other)
{
}

Models::~Models()
{
}

bool Models::Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char * modelFilename, char* textureFilename)
{
	bool result;

	// Load in the model data,
	result = LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}

	// Load the texture for this model.
	// Initialize the vertex and index buffers.
	result = InitialiseBuffers(device);
	if (!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, deviceContext, textureFilename);
	if (!result)
	{
		return false;
	}
	

	return true;
}

void Models::Shutdown()
{
	ReleaseTexture();
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();
	// Release the model data.
	ReleaseModel();

	return;
}

void Models::Render(ID3D11DeviceContext *deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int Models::GetVertexCount()
{
	return m_vertexCount;
}

int Models::GetInstanceCount()
{
	return m_instanceCount;
}

ID3D11ShaderResourceView * Models::GetTexture()
{
	return m_Texture->GetTexture();
}

bool Models::InitialiseBuffers(ID3D11Device *device)
{
	VertexType* vertices;
	InstanceType* instances;
	D3D11_BUFFER_DESC vertexBufferDesc, instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, instanceData;
	HRESULT result;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.

	/*//// Load the vertex array with data.
	//vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	////vertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	//vertices[0].texture = XMFLOAT2(0.0f, 1.0f);
	//vertices[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	//vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top middle.
	////vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	//vertices[0].texture = XMFLOAT2(0.0f, 1.0f);

	//vertices[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	//vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	//vertices[0].texture = XMFLOAT2(0.0f, 1.0f);

	////vertices[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	//vertices[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	
	// Load the index array with data.
	//indices[0] = 0;  // Bottom left.
	//indices[1] = 1;  // Top middle.
	//indices[2] = 2;  // Bottom right.*/

	// Load the vertex array and index array with data.
	for (int i = 0; i< m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		//indices[i] = i;
	}


					 // Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	//indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	//indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//indexBufferDesc.CPUAccessFlags = 0;
	//indexBufferDesc.MiscFlags = 0;
	//indexBufferDesc.StructureByteStride = 0;

	//// Give the subresource structure a pointer to the index data.
	//indexData.pSysMem = indices;
	//indexData.SysMemPitch = 0;
	//indexData.SysMemSlicePitch = 0;

	//// Create the index buffer.
	//result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	//if (FAILED(result))
	//{
	//	return false;
	//}
	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	/*delete[] indices;
	indices = 0;*/

	//INSTANCING
	// Set the number of instances in the array.
	m_instanceCount = 4;

	// Create the instance array.
	instances = new InstanceType[m_instanceCount];
	if (!instances)
	{
		return false;
	}

	// Load the instance array with data.
	instances[0].position = XMFLOAT3(-1.5f, 0.0f, 5.0f);
	instances[1].position = XMFLOAT3(0.0f, 0.0f, 5.0f);
	instances[2].position = XMFLOAT3(1.0f, -0.0f, 5.0f);
	instances[3].position = XMFLOAT3(1.5f, 0.0f, 5.0f);

	

	// Set up the description of the instance buffer.
	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType) * m_instanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = 0;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the instance data.
	instanceData.pSysMem = instances;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	// Create the instance buffer.
	result = device->CreateBuffer(&instanceBufferDesc, &instanceData, &m_instanceBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the instance array now that the instance buffer has been created and loaded.
	delete[] instances;
	instances = 0;

	return true;
}

void Models::ShutdownBuffers()
{
	// Release the index buffer.
	/*if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}
*/
	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	//Release the instance buffer
	if (m_instanceBuffer)
	{
		m_instanceBuffer->Release();
		m_instanceBuffer = 0;
	}

	return;
}

void Models::RenderBuffers(ID3D11DeviceContext *deviceContext)
{
	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];

	// Set the buffer strides.
	strides[0] = sizeof(VertexType);
	strides[1] = sizeof(InstanceType);

	// Set the buffer offsets.
	offsets[0] = 0;
	offsets[1] = 0;

	// Set the array of pointers to the vertex and instance buffers.
	bufferPointers[0] = m_vertexBuffer;
	bufferPointers[1] = m_instanceBuffer;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);


	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool Models::LoadTexture(ID3D11Device *device, ID3D11DeviceContext *deviceContext, char *filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new Texture;
	if (!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialise(device, deviceContext, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void Models::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

bool Models::ExportModel()
{
	//open file for writing
	//ofstream file;
	//file.open("../DirectyXTry/Assets/exportTxt.txt");
	//if (file.fail())
	//{
	//	return false;
	//}
	////.txt file format
	//if (m_model)
	//{
	//	file << "Vertex Count: " << m_vertexCount;
	//	file << "\nData: \n";
	//	for (int i = 0; i < m_vertexCount; i++)
	//	{
	//		file << m_model[i].x << m_model[i].y << m_model[i].z << endl;
	//		file << m_model[i].tu << m_model[i].tv;
	//		file << m_model[i].nx << m_model[i].ny << m_model[i].nz;
	//		file.close();
	//	}
	//}

	ofstream file;
	file.open("../DirectyXTry/Assets/exportObj.txt");
	file.clear();
	if (file.fail())
	{
		return false;
	}
	//.obj file format
	if (m_model)
	{
		file << "#.Obj export file: \n";
		file << "#Vertex count: " << m_vertexCount << endl;
		//adding a new group
		//file << "\ng myTri \n";
		file << "\n#Vertices: \n";
		for (int i = 0; i < m_vertexCount; i++)
		{
			file << "v "  << m_model[i].x << " " << m_model[i].y << " " << m_model[i].z << endl;
		}
		file << "\n#Vertex Uvs: \n";
		for (int i = 0; i < m_vertexCount; i++)
		{
			file << "vt " << m_model[i].tu << " " << m_model[i].tv << endl;
		}
		file << "\n#Vertex Normals: \n";
		for (int i = 0; i < m_vertexCount; i++)
		{
			file << "vn " << m_model[i].nx << " " << m_model[i].ny << " " << m_model[i].nz << endl;
		}
		file << "\n#Faces: \n";
		file << "f " << "3//1 2//1 1//1" << endl;
 		file.close();
	}
	return true;
}

bool Models::LoadModel(char* filename)
{
	ifstream file;
	char input;

	// Open the model file.
	file.open(filename);

	// If it could not open the file then exit.
	if (file.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	file.get(input);
	while (input != ':')
	{
		file.get(input);
	}

	// Read in the vertex count.
	file >> m_vertexCount;

	//// Set the number of indices to be the same as the vertex count.
	//m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	// Read up to the beginning of the data.
	file.get(input);
	while (input != ':')
	{
		file.get(input);
	}
	file.get(input);
	file.get(input);

	// Read in the vertex data.
	for (int i = 0; i<m_vertexCount; i++)
	{
		file >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		file >> m_model[i].tu >> m_model[i].tv;
		file >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	// Close the model file.
	file.close();

	return true;
}

void Models::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}
