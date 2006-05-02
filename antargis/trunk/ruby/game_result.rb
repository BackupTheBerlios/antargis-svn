class GameResult
	attr_reader :won
	def initialize(pWon="canceled")
		@won=pWon
	end
end