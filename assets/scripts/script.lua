

function update(self, dt)
	print("STAGED")
end

function init(self)
	print("You'll never feel happy! Never feel happy! No you won't... until you try");
	print("UpdateFunc type: " .. type(self.updateFunc) .. "     update type: " .. type(update))
	--self.updateFunc = update;
end