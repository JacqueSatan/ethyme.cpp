#include "structures/message.hpp"

#include "client.hpp"
#include "structures/channels/textchannel.hpp"
#include "structures/user.hpp"

namespace Ethyme::Structures
{
	Message::Message(nlohmann::json const& data, Ethyme::Client const& client)
		: m_content{ data["content"].get<std::string>() }
		, Entity{ data["id"].get<std::string>(), client }
		, m_author{ User(data["author"], client) }
		, m_channel{ client.Channels().FindById(data["channel_id"])->As<TextChannel>() }
	{
	}

	Structures::User const& Message::Author() const { return m_author; }
	Structures::TextChannel const& Message::Channel() const { return m_channel; }
	std::string const& Message::Content() const { return m_content; }

	cppcoro::task<Message const&> Message::Delete()
	{
		cpr::Delete(
			cpr::Url{ Constants::API::Channels + Channel().Id().ToString() + "/messages/" + Id().ToString() },
			cpr::Header{
				{ "Authorization", this->Client().Token() },
				{ "Content-Type", "application/json" }
			}
		);

		Logger::Debug("Message deleted in channel " + Channel().Id().ToString());

		co_return *this;
	}
	// Structures::Mentions const& Message::Mentions() const { return m_mentions; }
}