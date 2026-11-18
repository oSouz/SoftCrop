# Inspeção estática — SoftCropper_FFGL21.dll

## Arquivo efetivamente inspecionado
O caminho `/mnt/data/SoftCropper_FFGL21.dll` veio de outro ambiente. O anexo da conversa foi localizado no cache local e inspecionado ali. Nenhuma carga ou execução da DLL proprietária foi realizada.

- Tamanho: **601.088 bytes**.
- SHA-256: `1bcb84847b36423b277722408452a001826c809331f87c007ff7d4f5cbd79afc`.
- Formato: PE32+, máquina `0x8664` — Windows AMD64/x64.
- Exportação nomeada: **plugMain**, compatível com a entrada pública FFGL.
- Timestamp PE: 1583236084, equivalente a 2020-03-03 11:48:04 UTC. É um campo informativo alterável, não prova da data de produção.
- Diretório de certificados Authenticode ausente na tabela PE; não foi realizada verificação de reputação ou autenticidade.
- Importações incluem OPENGL32.dll, MSVCP140.dll, VCRUNTIME140.dll, bibliotecas UCRT e bibliotecas do Windows. São coerentes com um plugin C++ que usa OpenGL. A lista completa está em `analysis/original-pe.json`.

## Strings confirmadas no binário

| Grupo | Strings exatas |
|---|---|
| Identificação | SoftCropper; Haute Technique |
| Recortes | Crop Left; Crop Top; Crop Right; Crop Bottom |
| Suavizações | Soft Left; Soft Top; Soft Right; Soft Bottom |
| Outros | Style; Multiply |
| Identificadores | cropL, cropT, cropR, cropB; sizeL, sizeT, sizeR, sizeB; style |

Também há nomes RTTI relacionados a FFGL, JUCE e Boost. Isso indica referências a essas classes/bibliotecas; não revela, por si só, sua função no efeito.

## Interpretação e limites
Há evidência forte de um efeito de recorte por quatro lados, com suavização independente das bordas. Essa interpretação vem do nome e dos rótulos legíveis, não de uma observação de sua imagem.

Não foram confirmados por chamadas à API: versão declarada FFGL, ID do plugin, quantidade/ordem efetiva dos parâmetros, tipos, intervalos, valores padrão ou opções de Style. O sufixo FFGL21 é compatível com a intenção de FFGL 2.1, mas o nome do arquivo não prova a versão da API. `Multiply` pode ser uma opção ou rótulo; não permite inferir o conjunto de modos nem a fórmula usada. Nenhuma equivalência pixel a pixel é reivindicada.

## Implementação original proposta
Souz Soft Crop usa uma entrada e uma máscara retangular calculada no shader. Os cortes secos são simétricos em pares horizontal e vertical, com faixa interna de suavização simétrica por eixo; as quatro máscaras podem ser combinadas por produto ou mínimo e interpoladas pelo controle Corner Blend. A operação atua nos quatro canais e respeita o tamanho útil da textura FFGL.

Nome exibido na versão atual: Souz SoftCrop 14. ID: SZC4. API declarada e consultada: 2.1. Versão do plugin: 1.4. São cinco parâmetros: dois cortes por eixo, duas suavizações por eixo e Corner Blend. A identidade foi alterada para distinguir esta versão das instâncias anteriores. Não há compatibilidade de presets com o binário proprietário nem migração automática de instâncias Souz antigas.

## Fontes públicas consultadas
- SDK oficial, versão fixada: https://github.com/resolume/ffgl/tree/v2.1
- Releases oficiais: https://github.com/resolume/ffgl/releases
- Cabeçalhos e fontes públicos incluídos no SDK, especialmente FFGL.h, FFGLPluginSDK.h e utilitários ffglex.

## Reproduzir a inspeção

```powershell
python tools/inspect_pe.py "C:/caminho/SoftCropper_FFGL21.dll"
```

O script lê o cabeçalho PE, importações, exportações, hash e uma seleção explícita de strings. Não descompila nem executa o arquivo.

## Comparação com o GridPanel do usuário

A implementação de thumbnail em GridPanel/Thumbnail.h usa CFFGLThumbnailInfo com 640×360 e palavras 0xRRGGBBAA. O plugin é registrado como FF_EFFECT, sem código observado para impor uma thumbnail de clipe. O SoftCrop 1.3 adota essas mesmas dimensões e representação numérica. O SoftCrop 1.2 usava uma representação diferente, 0xAABBGGRR; os testes antigos conferiam essa representação própria, não sua concordância com o GridPanel/SDK. A consulta da API confirma os novos pixels; a exibição no host continua pendente.
