#include "Mesh.h"



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Mesh Constructor that assigns a Device to the Mesh
Parameters:
[in] pDevice - D3D Device
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Mesh::Mesh(LPDIRECT3DDEVICE9 pDevice) : pDevice(pDevice)
{
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Destructor
No Parameters.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Mesh::~Mesh()
{
    //for (int i = 0; i < Textures.size(); i++)
    //{
    //    if (Textures[i])
    //        Textures[i]->Release();
    //}
    //if(pMesh)
    //    pMesh->Release();
}





/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Renders the mesh
No Parameters.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Mesh::Render()
{
    //if (pDevice && pMesh)
    {

        pDevice->SetTransform(D3DTS_WORLD, GetTransform());
        int numMaterials = GetNumMaterials();
        for (int i = 0; i < numMaterials; i++)
        {
            pDevice->SetMaterial(GetMaterial(i));
            pDevice->SetTexture(0, GetTexture(i));
            GetMesh()->DrawSubset(i);
        }
    }
}

void Mesh::Render(D3DXMATRIX* T, D3DXMATRIX* R) {
    D3DXMATRIX W;
    W = *GetTransform();
    if (
        (T->_41 > 0 && W._41 > -1 * T->_41) ||
        (T->_41 < 0 && W._41 < -1 * T->_41) ||
        (T->_42 > 0 && W._42 > -1 * T->_42) ||
        (T->_42 < 0 && W._42 < -1 * T->_42) ||
        (T->_43 > 0 && W._43 > -1 * T->_43) ||
        (T->_43 < 0 && W._43 < -1 * T->_43)) {
        return;
    }
    W = W * *T * *T * *R;
    pDevice->SetTransform(D3DTS_WORLD, &W);
    int numMaterials = GetNumMaterials();
    for (int i = 0; i < numMaterials; i++)
    {
        pDevice->SetMaterial(GetMaterial(i));
        pDevice->SetTexture(0, GetTexture(i));
        GetMesh()->DrawSubset(i);
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Tries to load a mesh on current and parent folder
Parameters:
[in] folder - the folder path of the file
[in] fileName - file name
Returns: TRUE if load was successful, FALSE otherwise
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool Mesh::Load(char* folder, char* fileName)
{
    const int lenFolder = lstrlen(folder);
    TCHAR file[MAX_PATH];
    lstrcpyn(file, folder, MAX_PATH);
    lstrcpyn(file + lenFolder, fileName, MAX_PATH - lenFolder);
    const TCHAR* strPrefix = TEXT("..\\");
    const int lenPrefix = lstrlen(strPrefix);
    LPD3DXBUFFER pMaterialBuffer;
    DWORD numMaterials;
    HRESULT hr = D3DXLoadMeshFromX(file, D3DXMESH_SYSTEMMEM, pDevice, NULL, &pMaterialBuffer, NULL, &numMaterials, &pMesh);
    if (FAILED(hr))
    {
        TCHAR strFile[MAX_PATH];
        lstrcpyn(strFile, strPrefix, MAX_PATH);
        lstrcpyn(strFile + lenPrefix, file, MAX_PATH - lenPrefix);
        hr = D3DXLoadMeshFromX(strFile, D3DXMESH_SYSTEMMEM, pDevice, NULL, &pMaterialBuffer, NULL, &numMaterials, &pMesh);
        if (FAILED(hr)) {
            SetError("D3DXLoadMeshFromX - Failed");
            return FALSE;
        }
    }

    // Store material and texture information
    D3DXMATERIAL* pMaterials = (D3DXMATERIAL*)pMaterialBuffer->GetBufferPointer();

    // Copy the materials and textures from the buffer to the member arrays
    for (DWORD i = 0; i < numMaterials; i++)
    {
        Materials.push_back(pMaterials[i].MatD3D);
        Materials[i].Ambient = Materials[i].Diffuse;
        // Create the texture if it exists
        Textures.push_back(NULL);
        if (pMaterials[i].pTextureFilename != NULL &&
            lstrlen(pMaterials[i].pTextureFilename) > 0)
        {
            TCHAR texturePath[MAX_PATH];
            lstrcpyn(texturePath, folder, MAX_PATH);
            lstrcpyn(texturePath + lenFolder, pMaterials[i].pTextureFilename, MAX_PATH - lenFolder);
            hr = D3DXCreateTextureFromFile(pDevice,
                texturePath,
                &Textures[i]);
            // Create the texture
            if (FAILED(hr))
            {
                // If texture is not in current folder, try parent folder
                TCHAR strTexture[MAX_PATH];
                lstrcpyn(strTexture, strPrefix, MAX_PATH);
                lstrcpyn(strTexture + lenPrefix, texturePath, MAX_PATH - lenPrefix);
                hr = D3DXCreateTextureFromFile(pDevice,
                    strTexture,
                    &Textures[i]);
                if (FAILED(hr))
                {
                    SetError("D3DXCreateTextureFromFile - Failed");
                    return FALSE;
                }
            }
        }
    }

    // Release the buffer because we are done with it!
    pMaterialBuffer->Release();


    BYTE* v = 0;
    pMesh->LockVertexBuffer(0, (void**)&v);
    D3DXComputeBoundingSphere(
        (D3DXVECTOR3*)v,
        pMesh->GetNumVertices(),
        D3DXGetFVFVertexSize(pMesh->GetFVF()),
        &originalBoundingSphere._center,
        &originalBoundingSphere._radius);

    pMesh->UnlockVertexBuffer();

    return TRUE;
}