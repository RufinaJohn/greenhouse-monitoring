import {React,useState,useEffect}  from 'react';
import axios from 'axios';
import * as moment from 'moment-timezone';

function App() {
  const [data,setData] = useState([]);
  const [update_time,setUpdate_time] =useState(null);
  const [light,setLight] = useState('off');
  const [temp,setTemp] = useState(0);
  const [humidity, setHumidity] = useState(0);
  const [distance, setDistance] = useState(0);

  const getData = async () => {
    try 
    {
      const response = await axios.get('https://api.thingspeak.com/channels/2497276/feeds.json',{params:{api_key:"QLG7E303GRTBH8HA",results:1}});
      setData(response.data);

      const updateTimeInUTC = moment.parseZone(response.data.channel.updated_at);
      const formattedUpdateTime = updateTimeInUTC.tz("Asia/Kolkata").format('DD-MM-YYYY hh:mm:ss A'); // Adjust the format as needed (e.g., 12-hour vs. 24-hour clock)
      setUpdate_time(formattedUpdateTime);

      setLight(response.data.feeds[0].field1==0?'on':'off')

      setTemp(response.data.feeds[0].field2);

      setHumidity(response.data.feeds[0].field3);

      setDistance(400-response.data.feeds[0].field4);
      
      console.log(response.data)
    } 
    catch (error) 
    {
      console.error('Fetching Stocks failed:', error);
    }
  };

  useEffect(() => {
    getData();
  }, [data]);
  return (
    <div className="App">
     <p>Channel last updated at : {update_time}</p>
     <p>Light : {light}</p>
     <p>Temperature : {temp} degrees Celsius</p>
     <p>Humidity : {humidity} %</p>
     <p>Water level in tank : {distance} cm</p>
    </div>
  );
}

export default App;
