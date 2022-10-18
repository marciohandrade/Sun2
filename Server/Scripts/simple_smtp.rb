require 'net/smtp'

class SimpleSmtp
  attr_accessor :smtp_host, :from, :from_alias, :to, :to_alias, :subject, :message
  def initialize()
  end
  def send()
    msg = <<END_OF_MESSAGE
From: #{@from_alias} <#{@from}>
To: #{@to_alias} <#{@to}>
Subject: #{@subject}

#{@message}
END_OF_MESSAGE
    Net::SMTP.start(@smtp_host) do |smtp|
      smtp.send_message(msg, @from, @to)
    end
  end
end