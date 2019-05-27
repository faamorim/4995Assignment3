#include "Mirror.h"





const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

bool Mirror::setup(LPDIRECT3DDEVICE9 pDevice) {
    HRESULT r;
    this->pDevice = pDevice;

    r = pDevice->CreateVertexBuffer(
        36 * sizeof(Vertex),
        0, // usage
        Vertex::FVF,
        D3DPOOL_MANAGED,
        &VB,
        0);


    Vertex points[8];
    points[0] = Vertex(-2.5f, +0.0f, -2.5f);
    points[1] = Vertex(-2.5f, +5.0f, -2.5f);
    points[2] = Vertex(+2.5f, +5.0f, -2.5f);
    points[3] = Vertex(+2.5f, +0.0f, -2.5f);
    points[4] = Vertex(+2.5f, +0.0f, +2.5f);
    points[5] = Vertex(+2.5f, +5.0f, +2.5f);
    points[6] = Vertex(-2.5f, +5.0f, +2.5f);
    points[7] = Vertex(-2.5f, +0.0f, +2.5f);

    faces[0] = Vertex(0.0f, 0.0f, 2.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f); //Front Face
    faces[1] = Vertex(0.0f, 0.0f, -2.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f); //Back Face
    faces[2] = Vertex(2.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f); //Left Face
    faces[3] = Vertex(-2.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f); //Right Face
    faces[4] = Vertex(0.0f, -5.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f); //Top Face
    faces[5] = Vertex(0.0f, 0.0001f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f); //Bottom Face




    Vertex* v = 0;
    r = VB->Lock(0, 0, (void**)&v, 0);


    //
    //        6---------------5
    //        /|            /|
    //       / |           / |
    //      /  |          /  |
    //    1|-------------|2  |
    //     |   |         |   |
    //     |  7----------|----4
    //     |  /          |  /
    //     | /           | /
    //     |/            |/
    //    0---------------3


    // mirror
    //Front Face
    v[0] = Vertex(points[0], 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
    v[1] = Vertex(points[1], 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
    v[2] = Vertex(points[2], 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

    v[3] = Vertex(points[3], 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
    v[4] = v[0];
    v[5] = v[2];

    //Back Face
    v[6] = Vertex(points[4], 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
    v[7] = Vertex(points[5], 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
    v[8] = Vertex(points[6], 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

    v[9] = Vertex(points[7], 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
    v[10] = v[6];
    v[11] = v[8];

    //Left Face
    v[12] = Vertex(points[7], -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    v[13] = Vertex(points[6], -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    v[14] = Vertex(points[1], -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    v[15] = Vertex(points[0], -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    v[16] = v[12];
    v[17] = v[14];

    //Right Face
    v[18] = Vertex(points[3], 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    v[19] = Vertex(points[2], 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    v[20] = Vertex(points[5], 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    v[21] = Vertex(points[4], 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    v[22] = v[18];
    v[23] = v[20];

    //Top Face
    v[24] = Vertex(points[1], 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
    v[25] = Vertex(points[6], 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
    v[26] = Vertex(points[5], 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

    v[27] = Vertex(points[2], 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
    v[28] = v[24];
    v[29] = v[26];

    //Bottom Face
    v[30] = Vertex(points[7], 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
    v[31] = Vertex(points[0], 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
    v[32] = Vertex(points[3], 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

    v[33] = Vertex(points[4], 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
    v[34] = v[30];
    v[35] = v[32];
    

    r = VB->Unlock();


    r = pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    r = pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    r = pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

    r = pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

    r = D3DXCreateTextureFromFile(pDevice, "ice.bmp", &MirrorTex);
    

    return true;
}

void Mirror::RenderMirror(std::vector<Mesh>* pmeshes, size_t size)
{
    HRESULT r;

    D3DXMATRIX I;
    D3DXMatrixIdentity(&I);
    pDevice->SetTransform(D3DTS_WORLD, &I);
    pDevice->SetStreamSource(0, VB, 0, sizeof(Vertex));
    pDevice->SetFVF(Vertex::FVF);

    r = pDevice->SetMaterial(&MirrorMtrl);
    r = pDevice->SetTexture(0, MirrorTex);
    r = pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 12);

    //
    // Draw Mirror quad to stencil buffer ONLY.  In this way
    // only the stencil bits that correspond to the mirror will
    // be on.  Therefore, the reflected teapot can only be rendered
    // where the stencil bits are turned on, and thus on the mirror 
    // only.
    //

    r = pDevice->SetRenderState(D3DRS_STENCILENABLE, true);
    r = pDevice->SetRenderState(D3DRS_STENCILREF, 0x1);
    r = pDevice->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
    r = pDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
    r = pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
    r = pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);

    // disable writes to the depth and back buffers
    r = pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);


    D3DXMatrixIdentity(&I);
        
    for (int mirror = 0; mirror < 6; mirror++) {
    // draw the mirror to the stencil buffer
    r = pDevice->SetStreamSource(0, VB, 0, sizeof(Vertex));
    r = pDevice->SetFVF(Vertex::FVF);
    r = pDevice->SetMaterial(&MirrorMtrl);
    r = pDevice->SetTexture(0, MirrorTex);
        int face = mirror;

        r = pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
        r = pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
        r = pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
        r = pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
        r = pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
        r = pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

        r = pDevice->SetTransform(D3DTS_WORLD, &I);
        r = pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 6*face, 2);

        // re-enable depth writes
        r = pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);

        // only draw reflected teapot to the pixels where the mirror
        // was drawn to.
        r = pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
        r = pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

        // position reflection
        D3DXMATRIX R, T;
        D3DXPLANE plane(faces[face]._nx, faces[face]._ny, faces[face]._nz, 0.0f); // xy plane
        D3DXMatrixReflect(&R, &plane);
        D3DXMatrixTranslation(&T, faces[face]._x, faces[face]._y, faces[face]._z);

        // clear depth buffer and blend the reflected meshes with the mirror
        r = pDevice->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
        r = pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
        r = pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
        r = pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);


        for (int i = 0; i < size; i++) {
            (*pmeshes)[i].Render(&T, &R);
        }

        pDevice->Clear(0, 0, D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, 0xff000000, 1.0f, 0L);
    }

    // Restore render states.
    r = pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
    r = pDevice->SetRenderState(D3DRS_STENCILENABLE, false);
    r = pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}