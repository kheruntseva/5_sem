const http = require('http');
const fs = require('fs').promises;
const path = require('path');

const server = http.createServer(async (req, res) => {
    if (req.url === '/png' && req.method === 'GET') {
        try {
            const filePath = path.join(__dirname, 'pic.png');
            const content = await fs.readFile(filePath);
            res.writeHead(200, { 'Content-Type': 'image/png' });
            res.end(content);
        } catch (err) {
            res.writeHead(500, { 'Content-Type': 'text/plain; charset=utf-8' });
            res.end('Ошибка при чтении файла: ' + err.message);
        }
    } else {
        res.writeHead(404, { 'Content-Type': 'text/plain; charset=utf-8' });
        res.end('Страница не найдена');
    }
});

const PORT = 5000;
server.listen(PORT, () => {
    console.log(`Сервер запущен на http://localhost:${PORT}`);
});