require 'spec'
require 'spec/story'
require 'ostruct'

Account=Struct.new(:balance)

class Account
  def transfer_to(account,amount)
    @balance-=amount
    account.balance+=amount
    amount
  end
end

#include Spec::Story::Extensions::Main
#run_story :type => RailsStory do |runner|

class AccountSteps < Spec::Story::StepGroup
  steps do |define|
    define.given("my savings account balance is $balance") do |balance|
      @savings_account = Account.new(balance.to_f)
    end

    define.given("my cash account balance is $balance") do |balance|
      @cash_account = Account.new(balance.to_f)
    end

    define.then("my savings account balance should be $expected_amount") do |expected_amount|
      @savings_account.balance.should == expected_amount.to_f
    end

    define.then("my cash account balance should be $expected_amount") do |expected_amount|
      @cash_account.balance.should == expected_amount.to_f
    end
    define.when("I transfer $amount") do |amount|
      @savings_account.transfer_to(@cash_account, amount.to_f)
    end
  end
end

steps = AccountSteps.new do |define|
  define.when("I transfer $amount") do |amount|
    @savings_account.transfer_to(@cash_account, amount.to_f)
  end
end

run_story do |runner|
  #runner.steps << LoginSteps.new
  #runner.steps << NavigationSteps.new
  runner.steps << AccountSteps.new
  runner.load File.expand_path(__FILE__).gsub(".rb","")
end
