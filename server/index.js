const express = require('express');
const app = express();
const PORT = 3000;

app.use(express.json());

app.post('/receive-json', (req, res) => {
  const receivedData = req.body;
  // Process receivedData
  console.log('Received JSON data:', receivedData);
  res.json({ message: 'Data received successfully' });
});

app.listen(PORT, () => {
  console.log(`Server is running on port ${PORT}`);
});