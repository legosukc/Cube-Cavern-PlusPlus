local Input = Game.Input

local Graphics = Game.Graphics

local VertexArray = Graphics.VertexArray
local VertexBuffer = Graphics.VertexBuffer
local ElementBuffer = Graphics.ElementBuffer
local Program = Graphics.Program
local Shader = Graphics.Shader
local Texture = Graphics.Texture

local SyncedModifierCollection = Assets.MiscClassesGlobal.SyncedModifierCollection

local Inventory = {
    MaxSlots = SyncedModifierCollection.new();
    BaseMaxSlots = 10;

    SelectedHotbarSlot = 1;
    HeldItem = nil;

    Items = {};
}

Inventory.Items[1] = Assets.ItemClassesClient.Dagger.new()
Inventory.HeldItem = Inventory.Items[1]

do
    local SlotTexture = Texture.new()
    SlotTexture:Bind()

    Texture.Load2DImageFromFile("Assets/inventory-slot-corner.png")

    Texture.SetFilteringUpscale(Graphics.TextureFiltering.Nearest)
    Texture.SetFilteringDownscale(Graphics.TextureFiltering.Nearest)

    Texture.SetWrappingHorizontal(Graphics.TextureWrapping.MirroredRepeat)
    Texture.SetWrappingVertical(Graphics.TextureWrapping.MirroredRepeat)
    
    
    local SlotVAO = VertexArray.new()
    local SlotVBO = VertexBuffer.new()
    local SlotEBO = ElementBuffer.new()

    SlotVAO:Bind()
    SlotVBO:Bind()
    SlotEBO:Bind()

    VertexBuffer.CopyFromBuffer(Graphics.SampleModels.FlatQuad.Vertices)
	ElementBuffer.CopyFromBuffer(Graphics.SampleModels.FlatQuad.Indices)

	VertexArray.EnableAttribute(0)
	VertexArray.SetAttributePointer(0, 2, Graphics.Types.Float, false, 4 * 4, 0)

	VertexArray.EnableAttribute(1)
	VertexArray.SetAttributePointer(1, 2, Graphics.Types.Float, false, 4 * 4, 4 * 2)

	VertexArray.Unbind()


    local SlotProgram = Program.new()
    do
        local VertexShader = Shader.new(Graphics.ShaderTypes.VertexShader)
        VertexShader:LoadShaderSourceFromFile("Shaders/InventorySlot.vrt")
        VertexShader:Compile()

        local FragmentShader = Shader.new(Graphics.ShaderTypes.FragmentShader)
        FragmentShader:LoadShaderSourceFromFile("Shaders/InventorySlot.frg")
        FragmentShader:Compile()

        SlotProgram:AttachShader(VertexShader)
		SlotProgram:AttachShader(FragmentShader)
	
		SlotProgram:Link()

		SlotProgram:DetachAllShaders()
    end

    SlotProgram:Use()

    local SlotTextureUniform = SlotProgram:GetUniformIndex("SlotTexture")
    local TexcoordScaleUniform = SlotProgram:GetUniformIndex("TexcoordScale")

    Program.SetUniformVec2(TexcoordScaleUniform, 2.0, 2.0)
    Program.SetUniformSint32(SlotTextureUniform, 0)

    Inventory.GraphicsObjects = {
        SlotVAO = SlotVAO;
        SlotVBO = SlotVBO;
        SlotEBO = SlotEBO;
        SlotIndexCount = Graphics.SampleModels.FlatQuad.IndiceCount;
        SlotProgram = SlotProgram;
        SlotTexture = SlotTexture;

        SlotProgramSlotTextureUniform = SlotTextureUniform;
        SlotProgramTexcoordScaleUniform = TexcoordScaleUniform;
        SlotProgramOffsetUniform = SlotProgram:GetUniformIndex("Offset");
        SlotProgramAspectRatioReciprocalUniform = SlotProgram:GetUniformIndex("AspectRatioReciprocal")
    }
end

local SlotBindings = {
    Input.GetBinding("Slot1", Enums.Scancode.One);
    Input.GetBinding("Slot2", Enums.Scancode.Two);
    Input.GetBinding("Slot3", Enums.Scancode.Three);
    Input.GetBinding("Slot4", Enums.Scancode.Four);
    Input.GetBinding("Slot5", Enums.Scancode.Five);
    Input.GetBinding("Slot6", Enums.Scancode.Six);
    Input.GetBinding("Slot7", Enums.Scancode.Seven);
    Input.GetBinding("Slot8", Enums.Scancode.Eight);
    Input.GetBinding("Slot9", Enums.Scancode.Nine);
    Input.GetBinding("Slot0", Enums.Scancode.Zero);
}

function Inventory:Update()
    
    for Index, Binding in SlotBindings do
        if Binding:Pressed() then
            self.SelectedHotbarSlot = Index
            self.HeldItem = self.Items[Index]
            print("selected hotbar slot ", Index)
            break
        end
    end
end

function Inventory:Draw()

    Graphics.EnableBlending()

    local GraphicsObjects = self.GraphicsObjects

    GraphicsObjects.SlotVAO:Bind()
    GraphicsObjects.SlotProgram:Use()


    Program.SetUniformFloat(GraphicsObjects.SlotProgramAspectRatioReciprocalUniform, Graphics.AspectRatioReciprocal)

    GraphicsObjects.SlotTexture:Bind()
    Texture.SetActiveTexture(0)

    Graphics.DrawElements(Graphics.DrawModes.Triangles, GraphicsObjects.SlotIndexCount, Graphics.Types.Uint8)

    Graphics.DisableBlending()
end

return Inventory