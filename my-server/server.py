from fastapi import FastAPI, HTTPException
from pydantic import BaseModel
from pymongo import MongoClient
from typing import List

app = FastAPI()

# 1. Kết nối tới MongoDB Compass
try:
    client = MongoClient("mongodb://localhost:27017/")
    db = client["SuperSystemDB"]
    collection = db["users"]
    print("👉 Kết nối MongoDB thành công!")
except Exception as e:
    print("❌ Lỗi kết nối MongoDB:", e)


# --- ĐỊNH NGHĨA CẤU TRÚC DỮ LIỆU (PYDANTIC) ---

class LoginData(BaseModel):
    username: str
    password: str

# Cấu trúc từng gói đăng ký dịch vụ con gửi từ Qt
class RegistrationItem(BaseModel):
    service_name: str
    total_price: str
    expiry_date: str

# Cấu trúc cụm danh sách dịch vụ đăng ký đính kèm username người dùng
class RegistrationData(BaseModel):
    username: str
    registrations: List[RegistrationItem]


# --- CÁC ROUTE API HỆ THỐNG ---

# 🔑 API 1: Xử lý Đăng Nhập
@app.post("/api/login")
async def login(data: LoginData):
    user = collection.find_one({"username": data.username, "password": data.password})
    if user:
        return {"success": True}
    else:
        return {"success": False}


# 📝 API 2: Xử lý Đăng Ký tài khoản
@app.post("/api/register")
async def register(data: LoginData):
    exist_user = collection.find_one({"username": data.username})
    if exist_user:
        return {"success": False, "message": "Tài khoản này đã tồn tại!"}
    
    new_user = {"username": data.username, "password": data.password}
    collection.insert_one(new_user)
    return {"success": True, "message": "Đăng ký thành công!"}


# 📺 API 3: Lấy danh sách dịch vụ nạp lên Ô Chọn (ComboBox) của Qt
@app.get("/api/get-services")
async def get_services():
    try:
        services_collection = db['services']
        services = list(services_collection.find({}, {'_id': 0})) 
        return services
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))


# 💾 API 4: Lưu thông tin đăng ký dịch vụ theo từng User vào collection riêng biệt
@app.post("/api/save-registration")
async def save_registration(data: RegistrationData):
    try:
        # Lấy thông tin gói đăng ký mới từ gói tin gửi lên
        # Vì Qt gửi mảng chứa 1 phần tử nên ta lấy phần tử đầu tiên [0]
        new_registration = data.registrations[0].dict()
        
        # Tìm xem user này đã từng đăng ký gói nào chưa:
        # - Nếu CÓ: $push sẽ đẩy gói mới vào mảng 'registrations' có sẵn
        # - Nếu CHƯA: upsert=True sẽ tự động tạo mới document cho user này
        db['user_registrations'].update_one(
            {"username": data.username},
            {"$push": {"registrations": new_registration}},
            upsert=True
        )
        return {"success": True}
    except Exception as e:
        print("❌ Lỗi lưu dữ liệu:", e)
        return {"success": False}

@app.get("/api/registrations")
async def get_registrations():
    try:
        registrations = list(
            db["user_registrations"].find({}, {"_id": 0})
        )
        return registrations
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="127.0.0.1", port=3000)