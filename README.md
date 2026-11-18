# Souz Toolkit — Souz Soft Crop 1.0

Efeito original de recorte retangular com suavização independente das quatro bordas, para Resolume/FFGL 2.1 Windows x64. Não contém código da DLL SoftCropper, não depende dela e não é produto da Haute Technique. Equivalência funcional proposta, não reprodução exata nem compatibilidade de presets.

## Downloads

- [DLL pronta para Windows x64](dist/SouzSoftCrop_FFGL21.dll)
- [Pacote completo com fonte e SDK](SouzToolkit_SoftCrop_FFGL21_v1.0.zip)
- [Relatório da inspeção](RELATORIO.md)

## Arquivos
- `dist/SouzSoftCrop_FFGL21.dll`: build Release x64.
- `src/SouzSoftCrop.cpp`: implementação original C++ e GLSL.
- `CMakeLists.txt`: compilação com SDK público oficial.
- `RELATORIO.md`: inspeção do arquivo de referência e limites das conclusões.
- `analysis/original-pe.json`: evidências estáticas selecionadas.
- `analysis/metadata-test.json`: teste do plugin novo.
- `tools/inspect_pe.py`: inspeção estática reproduzível, sem executar a DLL.
- `tests/check_metadata.py`: consulta da API pública da DLL nova.
- `vendor/ffgl`: SDK oficial v2.1 com avisos de copyright preservados.

## Parâmetros da implementação Souz
Todos variam de 0 a 1 e começam em 0. Uma entrada de vídeo; sem redimensionar ou deslocar o conteúdo.

| Índice | Nome | Comportamento |
|---|---|---|
| 0 | Crop Left | Fração da largura removida à esquerda |
| 1 | Crop Top | Fração da altura removida no topo |
| 2 | Crop Right | Fração da largura removida à direita |
| 3 | Crop Bottom | Fração da altura removida embaixo |
| 4 | Soft Left | Largura da transição, como fração da largura da imagem |
| 5 | Soft Top | Altura da transição, como fração da altura da imagem |
| 6 | Soft Right | Largura da transição à direita |
| 7 | Soft Bottom | Altura da transição embaixo |
| 8 | Corner Blend | 0: produto das quatro máscaras; 1: menor valor; intermediários interpolam |

A suavização usa uma curva cúbica smoothstep e avança da linha de recorte para dentro da área visível. Soft=0 produz borda dura. Recortes opostos que somam 1 ou mais tornam a imagem inteiramente transparente. Os canais RGBA são multiplicados pela máscara para preservar alfa pré-multiplicado. Sem recorte nem suavização, a saída é a entrada. Top é a borda superior (UV y=1). Não há antialias adicional na borda dura.

Exemplo: Crop Left=0,20 e Soft Left=0,10 removem os 20% iniciais e fazem a transição entre 20% e 30% da largura.

## Compilar
Requisitos: Windows x64, Visual Studio 2022 ou Build Tools com **Desenvolvimento para desktop com C++**, Windows SDK e CMake 3.20+. Python 3 de 64 bits é opcional para o teste.

Na pasta do projeto, em PowerShell:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release --parallel 4
python tests/check_metadata.py build/bin/Release/SouzSoftCrop_FFGL21.dll
```

Resultado: `build/bin/Release/SouzSoftCrop_FFGL21.dll`.

O SDK incluído corresponde a https://github.com/resolume/ffgl/tree/v2.1, commit `6697342f153cba9df9fd089a56d29ca87712fe30`. Para um checkout sem vendor, obtenha-o com:

```powershell
git clone --branch v2.1 --depth 1 https://github.com/resolume/ffgl.git vendor/ffgl
```

Não use master como substituição silenciosa: é outra versão do SDK. Também pode selecionar um SDK v2.1 local com `-DFFGL_SDK="C:/caminho/ffgl"` na configuração CMake.

## Instalar e experimentar no Resolume
1. Copie a DLL de `dist` para uma pasta de plugins própria do acervo Souz Toolkit.
2. Nas preferências de vídeo do Resolume, adicione essa pasta aos diretórios de plugins FFGL; faça nova varredura ou reinicie o programa conforme sua versão.
3. Procure **Souz Soft Crop** na lista de efeitos e aplique a um clipe com vídeo.
4. Teste cada recorte isoladamente e depois suas suavizações. Use fundo quadriculado ou outra camada atrás para verificar transparência.

O SDK v2.1 documenta suporte a suas funcionalidades a partir do Resolume 7.0.3. O shader exige OpenGL/GLSL 4.1. Build MSVC usa o runtime Visual C++ x64; caso o carregamento falhe por runtime ausente, instale o redistribuível oficial correspondente. Não foi instalado automaticamente nem testado em uma sessão do Resolume.

## Validação desta entrega
- Compilação Release com MSVC 19.44, Visual Studio 2022, Windows SDK 10.0.26100.
- DLL nova carregada por Python x64: API=2.1, ID=SZC1, nome e nove parâmetros/defaults confirmados; inicialização/desinicialização da API aprovadas.
- Não foram executados InitGL, compilação do shader em GPU ou comparação visual no host. O teste de metadados não certifica renderização. Esta entrega é uma versão inicial compilada para validação no Resolume.

Checklist visual: bypass em valores zero; cada borda na direção correta; suavização sem halo em fundo colorido; recortes cruzados transparentes; resolução/relação de aspecto diferentes; dois efeitos simultâneos sem interferência; salvar/reabrir composição. A largura da suavização é relativa a cada eixo, não uma distância uniforme em pixels.

## Autoria e dependências
O código do efeito foi escrito do zero a partir de operações matemáticas comuns e da API pública FFGL. Nenhum shader, trecho descompilado, recurso ou identidade binária do plugin de referência foi reutilizado. Os nomes descritivos dos oito controles orientaram a proposta. `Corner Blend` é uma decisão de projeto Souz, não uma afirmação sobre `Style`.

Veja `LICENSE` para o código novo e `THIRD_PARTY_NOTICES.md` para o SDK. O ID SZC1 foi escolhido para esta entrega; não há garantia de registro global, então mantenha-o único no acervo. A DLL proprietária não é distribuída neste pacote.
