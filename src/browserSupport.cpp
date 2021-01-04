
#include "browserSupport.h"

#include <string>


url::url(const std::string &urlText)
{
	this->urlText = urlText;
	urlValid = true;
}

url::url()
{
	urlText = "";
	urlValid = false;
}

url::~url()
{
}

void url::text(const std::string &urlText)
{
	this->urlText = urlText;
}

std::string url::text() const
{
	return urlText;
}

bool url::valid() const
{
	return urlValid;
}

bool url::operator< (const url& rhs) const
{
	return urlText< rhs.text();
}

bool url::operator==(const url& rhs) const
{
	return urlText==rhs.text();
}

url::operator std::string() const
{
	return text();
}

std::ostream& operator<<(std::ostream &out, const url &other)
{
	out << other.text() << std::endl;

	return out;
}

std::vector<url> *browserSupport::pVector = nullptr;

browserSupport::browserSupport()
{
	backURL = stack<url>();
	forwardURL = stack<url>();
	historyQueue = queue<url>();
	maxHistory = 50;
	currentURL = url();
}

browserSupport::~browserSupport()
{
}

url browserSupport::back()
{
	if(!backURL.empty())
	{
		if(currentURL.valid())
		{
			forwardURL.push(currentURL);
		}

		currentURL = backURL.top();
		backURL.pop();
	}

	return currentURL;
}

url browserSupport::forward()
{
	if(!forwardURL.empty())
	{
		if(currentURL.valid())
		{
			backURL.push(currentURL);
		}

		currentURL = forwardURL.top();
		forwardURL.pop();
	}

	return currentURL;
}

void browserSupport::display(const url &newURL)
{
	if(currentURL.valid())
	{
		backURL.push(currentURL);
	}
	currentURL = newURL;

	if(historyQueue.empty() || currentURL!=historyQueue.back())
	{
		historyQueue.push(currentURL);

		if(historyQueue.size()>maxHistory)
		{
			historyQueue.pop();
		}
	}

	forwardURL.clear();

}

std::size_t browserSupport::clearHistory()
{
	std::size_t historySize = historyQueue.size();
	historyQueue.clear();
	return historySize;
}
std::size_t browserSupport::clearCache()
{
	std::size_t numberReturned = 0;
	if(currentURL.valid())
		numberReturned++;
	numberReturned = forwardURL.size() + backURL.size() + numberReturned;
	currentURL = url();
	forwardURL.clear();
	backURL.clear();

	return numberReturned;
}
std::size_t browserSupport::maximum() const
{
	return maxHistory;
}
void browserSupport::maximum(std::size_t newMax)
{
	maxHistory = newMax;
	while(historyQueue.size()>maxHistory)
	{
		historyQueue.pop();
	}
}

void browserSupport::buildHistory(const url &nextURL)
{
	if (pVector != nullptr)
	{
		(*pVector).push_back(nextURL);
	}
}

std::size_t browserSupport::history(std::vector<url> &historyURLs)
{
	historyURLs.clear();
	browserSupport::pVector = &historyURLs;

	historyQueue.traverse(&browserSupport::buildHistory);


	browserSupport::pVector = nullptr;

	return 0;
}

