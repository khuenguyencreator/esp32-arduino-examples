# Kiểm tra build tự động (GitHub Actions)

Mỗi lần có code mới, GitHub sẽ tự **biên dịch toàn bộ project PlatformIO** trong repo. Bài nào không build được sẽ bị đánh dấu ❌ ngay, trước khi người đọc blog tải về và gặp lỗi.

File cấu hình: [`.github/workflows/build.yml`](../.github/workflows/build.yml)

## CI chạy khi nào?

| Sự kiện | Chạy gì |
|---|---|
| Push lên `master` | Build tất cả project |
| Mở / cập nhật Pull Request | Build tất cả project |
| Bấm chạy tay (xem bên dưới) | Build tất cả, hoặc chỉ các project khớp bộ lọc |

Workflow **tự dò mọi thư mục có `platformio.ini`**. Thêm bài mới không cần sửa file workflow.

## Xem kết quả

1. Vào tab **Actions** của repo trên GitHub, chọn workflow **Build**.
2. Mỗi lần chạy có một job cho từng project, tên job là đường dẫn thư mục (ví dụ `02-Sensor-Module/19_LVGL`).
   - ✅ xanh: build thành công
   - ❌ đỏ: build lỗi. Bấm vào job, mở bước **Build** để xem log lỗi của trình biên dịch.
3. Job **Tất cả build** ở cuối tổng hợp kết quả: chỉ xanh khi mọi project đều xanh.

Một bài lỗi không làm dừng các bài khác (`fail-fast: false`), nên một lần chạy cho biết hết danh sách bài lỗi.

## Chạy tay một phần

Khi chỉ muốn kiểm tra nhanh một nhóm bài:

1. Tab **Actions** → **Build** → nút **Run workflow**.
2. Ô **filter** nhập một phần đường dẫn, ví dụ:
   - `02-Sensor-Module`: build cả nhóm module
   - `19_LVGL`: build riêng bài LVGL
   - để trống: build tất cả
3. Bấm **Run workflow**.

## Build thử trên máy trước khi push

Chạy đúng lệnh mà CI dùng:

```bash
pip install platformio
pio run -d 02-Sensor-Module/19_LVGL
```

Build tất cả project trên máy (Linux/macOS/Git Bash):

```bash
find . -name platformio.ini -not -path "*/.pio/*" -exec dirname {} \; | sort | while read p; do
  echo "== $p"; pio run -d "$p" -s || echo "LOI: $p"
done
```

## Bắt buộc build xanh mới được merge (tùy chọn)

Settings → Branches → **Add branch protection rule** cho `master` → bật **Require status checks to pass before merging** → chọn check **Tất cả build**.

Từ đó mọi Pull Request phải build thành công mới merge được.

## Thời gian và chi phí

- Repo public: GitHub Actions **miễn phí, không giới hạn phút**. Tài khoản free chạy song song tối đa 20 job.
- Lần đầu mỗi job phải tải toolchain ESP32 (vài trăm MB), mất khoảng 3–5 phút mỗi bài. Các lần sau dùng cache nên nhanh hơn nhiều.
- Cache tự làm mới khi có `platformio.ini` thay đổi.

## CI này kiểm tra được gì, không kiểm tra được gì

| Bắt được | Không bắt được |
|---|---|
| Lỗi cú pháp, sai tên hàm, sai API giữa các bản core | Code build được nhưng chạy sai logic |
| Thiếu thư viện, sai phiên bản trong `lib_deps` | Sai sơ đồ chân, sai timing phần cứng |
| Cấu hình `platformio.ini` sai (board, build_flags) | Lỗi chỉ xuất hiện khi chạy (crash, treo, watchdog) |

Muốn kiểm tra cả lúc chạy thì cần thêm mô phỏng Wokwi (dự kiến, xem [ROADMAP](../ROADMAP.md)) hoặc nạp thử lên board thật.

## Lỗi thường gặp

| Log báo | Nguyên nhân / cách sửa |
|---|---|
| `fatal error: xxx.h: No such file or directory` | Thiếu thư viện trong `lib_deps`, hoặc tên thư viện sai |
| `'xxx' was not declared in this scope` | Hàm không có trong phiên bản Arduino core đang dùng (core 2.x và 3.x khác API) |
| `UnknownPackageError` / `Could not find the package` | Tên hoặc phiên bản thư viện trong `lib_deps` không tồn tại trên registry |
| `region 'iram0_0_seg' overflowed` / `text section exceeds available space` | Code quá lớn so với partition, cần `board_build.partitions = huge_app.csv` |
