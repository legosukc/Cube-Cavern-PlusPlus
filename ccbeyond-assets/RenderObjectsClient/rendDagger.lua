local Graphics = Game.Graphics

local VertexArray = Graphics.VertexArray
local VertexBuffer = Graphics.VertexBuffer
local ElementBuffer = Graphics.ElementBuffer

local Texture = Graphics.Texture

local DaggerRenderObject = setmetatable({}, Assets.RenderObjectsClient.rendViewarm)

local DaggerVAO = VertexArray.new()
local DaggerVBO = VertexBuffer.new()
local DaggerEBO = ElementBuffer.new()

DaggerVAO:Bind()
DaggerVBO:Bind()
DaggerEBO:Bind()

local DaggerModel = ModelLoader.LoadFile("Models/Dagger.obj")

VertexBuffer.CopyFromBuffer(DaggerModel.VertexData)
ElementBuffer.CopyFromBuffer(DaggerModel.IndexData)

VertexArray.EnableAttribute(0)
VertexArray.SetAttributePointer(0, 3, Graphics.Types.Float, false, DaggerModel.StrideSize, 0)

VertexArray.EnableAttribute(1)
VertexArray.SetAttributePointer(1, 3, Graphics.Types.Float, false, DaggerModel.StrideSize, 4 * 3)

VertexArray.EnableAttribute(2)
VertexArray.SetAttributePointer(2, 2, Graphics.Types.Float, false, DaggerModel.StrideSize, 4 * 3 * 2)

VertexArray.Unbind()

DaggerRenderObject.VAO = DaggerVAO
DaggerRenderObject.VBO = DaggerVBO
DaggerRenderObject.EBO = DaggerEBO

DaggerRenderObject.IndexCount = DaggerModel.IndexCount



local DaggerTexture = Texture.new()
DaggerTexture:Bind()

Texture.Load2DImageFromFile("Textures/dagger_palette.bmp")

Texture.SetFilteringUpscale(Graphics.TextureFiltering.Nearest)
Texture.SetFilteringDownscale(Graphics.TextureFiltering.Nearest)

Texture.SetWrappingHorizontal(Graphics.TextureWrapping.Repeat)
Texture.SetWrappingVertical(Graphics.TextureWrapping.Repeat)

DaggerRenderObject.Texture = DaggerTexture

return DaggerRenderObject