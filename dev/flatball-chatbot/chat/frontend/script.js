document.addEventListener('DOMContentLoaded', () => {
    const chatBox = document.getElementById('chat-box');
    const userInput = document.getElementById('user-input');
    const sendBtn = document.getElementById('send-btn');

    let abortController = null; // To hold the controller for the current fetch request

    const addMessage = (text, sender, sources = []) => {
        const messageElement = document.createElement('div');
        messageElement.classList.add('message', `${sender}-message`);
        
        const textElement = document.createElement('p');
        textElement.innerText = text;
        messageElement.appendChild(textElement);

        if (sources.length > 0) {
            const sourcesElement = document.createElement('div');
            sourcesElement.classList.add('sources');
            
            const sourcesTitle = document.createElement('span');
            sourcesTitle.classList.add('sources-title');
            sourcesTitle.innerText = 'Sources:';
            sourcesElement.appendChild(sourcesTitle);

            sources.forEach(source => {
                const sourceItem = document.createElement('span');
                sourceItem.classList.add('source-item');
                sourceItem.innerText = `- ${source}`;
                sourcesElement.appendChild(sourceItem);
            });
            messageElement.appendChild(sourcesElement);
        }

        chatBox.appendChild(messageElement);
        chatBox.scrollTop = chatBox.scrollHeight;
    };

    const handleSend = async () => {
        const question = userInput.value.trim();
        if (!question) return;

        addMessage(question, 'user');
        userInput.value = '';
        sendBtn.disabled = true;

        abortController = new AbortController(); // Create a new controller for this request

        let timerInterval;
        const thinkingMessageElement = document.createElement('div');
        thinkingMessageElement.classList.add('message', 'bot-message');
        
        const thinkingContent = document.createElement('p');
        thinkingContent.classList.add('thinking-message');

        const spinner = document.createElement('div');
        spinner.classList.add('spinner');
        thinkingContent.appendChild(spinner);

        const timerText = document.createElement('span');
        let seconds = 0;
        timerText.innerText = 'Thinking... (esc to cancel)'; // Updated text
        thinkingContent.appendChild(timerText);
        
        timerInterval = setInterval(() => {
            seconds++;
            timerText.innerText = `Thinking... ${seconds}s (esc to cancel)`;
        }, 1000);

        thinkingMessageElement.appendChild(thinkingContent);
        chatBox.appendChild(thinkingMessageElement);
        chatBox.scrollTop = chatBox.scrollHeight;

        try {
            const response = await fetch('/query', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ question }),
                signal: abortController.signal, // Link the controller to the fetch request
            });

            if (!response.ok) {
                throw new Error(`HTTP error! status: ${response.status}`);
            }

            const data = await response.json();
            
            chatBox.removeChild(thinkingMessageElement);
            const sources = data.sources.map(s => `${s.source}, Page: ${s.page}`);
            addMessage(data.answer, 'bot', sources);

        } catch (error) {
            chatBox.removeChild(thinkingMessageElement);
            if (error.name === 'AbortError') {
                addMessage('Request canceled.', 'bot');
            } else {
                addMessage(`Sorry, something went wrong: ${error.message}`, 'bot');
            }
            console.error('Error fetching response:', error);
        } finally {
            clearInterval(timerInterval);
            sendBtn.disabled = false;
            userInput.focus();
            abortController = null; // Clear the controller
        }
    };

    sendBtn.addEventListener('click', handleSend);
    userInput.addEventListener('keydown', (e) => {
        if (e.key === 'Enter' && !e.shiftKey) {
            e.preventDefault();
            handleSend();
        }
    });

    // --- Global Escape Key Listener ---
    document.addEventListener('keydown', (e) => {
        if (e.key === 'Escape' && abortController) {
            abortController.abort();
        }
    });
});
