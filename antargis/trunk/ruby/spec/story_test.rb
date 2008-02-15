require 'rubygems'
require 'spec/story/runner/plain_text_story_runner.rb'

include Spec::Story::Runner

PlainTextStoryRunner.new('path/to/file')

 # PlainTextStoryRunner.new do |runner|
 #   runner.load 'path/to/file'
 # end
  