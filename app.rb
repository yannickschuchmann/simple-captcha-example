require 'sinatra'
require 'moneta'
require 'securerandom'
require 'byebug'
require 'random-word'
require 'sinatra/synchrony'
require 'os'

register Sinatra::Synchrony
use Rack::Session::Cookie, :secret => 'FURFM-SCHNAPS'
set :bind, '0.0.0.0'
set :port, 80
store = Moneta.new(:Memory)
ACCEPTED = "accepted_"

def create_captcha_text
  # RandomWord.adjs.next + " " + RandomWord.nouns.next
  RandomWord.adjs.next
end


get '/' do
  user_id = session['user_id'] = session[:user_id] || SecureRandom.hex
  if store.key?(ACCEPTED + user_id)
    erb :secured
  else
    store[user_id] = create_captcha_text unless store[user_id]

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

get '/reload' do
  user_id = session['user_id'] = session[:user_id] || SecureRandom.hex
  store[user_id] = create_captcha_text
  redirect to('/')
end
