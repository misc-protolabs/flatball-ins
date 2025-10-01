import os
import json
from datetime import datetime
from fastapi import FastAPI, Request
from fastapi.staticfiles import StaticFiles
from fastapi.responses import HTMLResponse
from pydantic import BaseModel
import uvicorn

from langchain_ollama.llms import OllamaLLM
from langchain_core.prompts import PromptTemplate
from langchain_core.runnables import RunnablePassthrough
from langchain_core.output_parsers import StrOutputParser
from langchain_chroma.vectorstores import Chroma
from langchain_ollama.embeddings import OllamaEmbeddings

# --- Configuration ---
CHROMA_PATH = "./.chroma_db"
EMBEDDING_MODEL = "nomic-embed-text"
GENERATION_MODEL = "llama3:8b"
QUERY_LOG_PATH = "../.log/query_logs" # Centralized log path

# Ensure query log directory exists
os.makedirs(QUERY_LOG_PATH, exist_ok=True)

# --- FastAPI App ---
app = FastAPI()

# --- RAG Chain Initialization ---
def get_rag_chain():
    embeddings = OllamaEmbeddings(model=EMBEDDING_MODEL)
    vector_store = Chroma(persist_directory=CHROMA_PATH, embedding_function=embeddings)
    retriever = vector_store.as_retriever()

    template = """Answer the question based only on the following context. If you don't know the answer, just say that you don't know. Don't try to make up an answer.\n\nContext:\n{context}\n\nQuestion: {question}\n"""
    prompt = PromptTemplate.from_template(template)
    ollama_llm = OllamaLLM(model=GENERATION_MODEL, num_predict=1024)

    # The chain for getting the answer
    chain = (
        {"context": retriever, "question": RunnablePassthrough()}
        | prompt
        | ollama_llm
        | StrOutputParser()
    )
    return chain, retriever

rag_chain, retriever_instance = get_rag_chain()

# --- API Models ---
class QueryRequest(BaseModel):
    question: str

class Source(BaseModel):
    source: str
    page: int

class QueryResponse(BaseModel):
    answer: str
    sources: list[Source]

# --- Query Logging Function ---
def save_query_response(question, retrieved_docs, answer):
    """Saves the question, sources, and answer to a timestamped markdown file."""
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    filename = os.path.join(QUERY_LOG_PATH, f"query-{timestamp}-response.md")

    sources_text = "\n".join([
        f"- **Source:** {os.path.basename(doc.metadata.get("source", "Unknown"))}, **Page:** {doc.metadata.get("page", "N/A")}"
        for doc in retrieved_docs
    ])

    content = f"""# Chatbot Query Response\n\n**Timestamp:** {datetime.now().strftime("%Y-%m-%d %H:%M:%S")}\n\n---\n\n## ❓ Question\n\n{question}\n\n---\n\n## 📚 Sources Used\n\n{sources_text}\n\n---\n\n## 💡 Answer\n\n{answer}\n"""

    with open(filename, 'w', encoding='utf-8') as f:
        f.write(content)
    # In a real app, you might log this to a proper logger, not just print
    print(f"📝 Answer saved to {filename}")

# --- API Endpoints ---
@app.post("/query", response_model=QueryResponse)
async def query_endpoint(query: QueryRequest):
    """Endpoint to ask a question to the RAG chatbot."""
    question = query.question
    
    # Get sources first
    retrieved_docs = retriever_instance.invoke(question)
    sources = []
    for doc in retrieved_docs:
        sources.append(Source(
            source=os.path.basename(doc.metadata.get("source", "Unknown")),
            page=doc.metadata.get("page", 0) # Default to page 0 if not available
        ))

    # Get the answer
    answer = rag_chain.invoke(question)
    
    # Save the query and response to log
    save_query_response(question, retrieved_docs, answer)

    return QueryResponse(answer=answer, sources=sources)

# --- Static Files ---
# This serves the frontend directory and must be mounted after all other API routes
app.mount("/", StaticFiles(directory="frontend", html=True), name="static")


if __name__ == "__main__":
    print("Starting FastAPI server...")
    print("Access the UI at http://127.0.0.1:8000")
    uvicorn.run(app, host="127.0.0.1", port=8000)
