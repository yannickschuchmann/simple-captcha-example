require 'sinatra'
require 'moneta'
require 'securerandom'
require 'byebug'
require 'random-word'
require 'sinatra/synchrony'
require 'os'

register Sinatra::Synchrony
use Rack::Session::Cookie, :secret => 'FURFM-SCHNAPS'

store = Moneta.new(:Memory)
ACCEPTED = "accepted_"

get '/' do
  user_id = session['user_id'] = session[:user_id] || SecureRandom.hex
  if store.key?(ACCEPTED + user_id)
    erb :secured
  else
    store[user_id] = RandomWord.adjs.next + " " + RandomWord.adjs.next unless store[user_id]

    if OS.mac?
      captcha_generator = "imp-captcha_osx"
    else
      captcha_generator = "imp-captcha"
    end

    `cd public/captchas && ../../opencv-files/build/#{captcha_generator} "#{store[user_id]}"`
    @phrase = store[user_id]
    erb :input
  end
end

get '/captcha' do
  user_id = session[:user_id]
  store[ACCEPTED + user_id] = true if store[user_id] == params[:captcha_input]
  redirect to('/')
end
