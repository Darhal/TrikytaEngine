local a

function OnEngineLoad()
	print("____________________________________________________________________")
	a = CreateSprite("assets/test.png", 464, 464, 400, 400)
end
AddEventHandler("OnEngineLoad", OnEngineLoad)

