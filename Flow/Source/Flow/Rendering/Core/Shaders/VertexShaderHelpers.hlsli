// Constant Buffers /////////////////////////////////////////////////////////////////

cbuffer CB_Transform : register(b0)
{
    matrix m_modelView;
    matrix m_modelViewProj;
}