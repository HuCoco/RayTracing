#pragma once

class ComputeShaderRenderer
{
protected:
    ComputeShaderRenderer();
    ~ComputeShaderRenderer();
public:
    static ComputeShaderRenderer* GetInstance();
    void Initialize();
    void Finalize();
};